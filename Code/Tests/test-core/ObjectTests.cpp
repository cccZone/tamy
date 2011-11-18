#include "core-TestFramework\TestFramework.h"
#include "core\Object.h"
#include "core\Property.h"
#include "core\PropertiesView.h"
#include "core\PropertyEditor.h"
#include "core\Serializer.h"
#include "core\SerializerImpl.h"
#include "core\Resource.h"
#include "core\ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   struct BaseObject : public Resource
   {
      DECLARE_STRUCT(BaseObject)

      int   m_val;

      BaseObject(int val = -1) : Resource( "BaseObject" ), m_val(val) {}
      virtual ~BaseObject() {}
   };
   BEGIN_OBJECT(BaseObject, Resource)
      PROPERTY_EDIT("some label", int, m_val)
   END_OBJECT()
   REGISTER_RTTI( BaseObject )
   // -------------------------------------------------------------------------

   struct DerivedObject : public BaseObject
   {
      DECLARE_STRUCT(DerivedObject)

      int m_val;

      DerivedObject(int val = -2, int baseVal = -1) : BaseObject(baseVal), m_val(val) {}
   };
   BEGIN_OBJECT(DerivedObject, BaseObject)
      PROPERTY_EDIT("some label", int, m_val)
   END_OBJECT()
   REGISTER_RTTI( DerivedObject )
   // -------------------------------------------------------------------------

   struct DependentObject : public Resource
   {
      DECLARE_STRUCT(DependentObject)

      BaseObject* m_obj;

      DependentObject(BaseObject* obj = NULL) : Resource( "DependentObject" ), m_obj(obj) {}
   };
   BEGIN_OBJECT(DependentObject, Resource)
      PROPERTY_EDIT("some label", BaseObject*, m_obj)
   END_OBJECT()
   REGISTER_RTTI( DependentObject )
   // -------------------------------------------------------------------------

   class AbstractClass : public Resource
   {
      DECLARE_CLASS(AbstractClass)

   private:
      int m_baseVal;

   public:
            AbstractClass( const std::string& resName = "AbstractClass" ) : Resource( resName ),  m_baseVal(-1) {}
      virtual ~AbstractClass() {}

      void setBaseVal(int val) { m_baseVal = val; }
      int getBaseVal() const { return m_baseVal; }

      virtual void setVal(int val) {}
      virtual int getVal() const { return -1; }
   };
   BEGIN_OBJECT(AbstractClass, Resource)
      PROPERTY_EDIT("baseVal", int, m_baseVal)
   END_OBJECT()
   REGISTER_RTTI( AbstractClass )
   // -------------------------------------------------------------------------

   class ImplementingClass : public AbstractClass
   {
      DECLARE_CLASS(ImplementingClass)

   private:
      int m_val;

   public:
      ImplementingClass() : AbstractClass( "ImplementingClass" ), m_val(-1) {}

      void setVal(int val) { m_val = val; }
      int getVal() const { return m_val; }
   };
   BEGIN_OBJECT(ImplementingClass, AbstractClass)
      PROPERTY_EDIT("val", int, m_val)
   END_OBJECT()
   REGISTER_RTTI( ImplementingClass )
   // -------------------------------------------------------------------------

   class RegularObject : public Object
   {
      DECLARE_CLASS(RegularObject)

   public:
      Object* m_parent;

      RegularObject() : m_parent( NULL ) {}

   };
   BEGIN_OBJECT(RegularObject, Object)
      PROPERTY_EDIT("parent", Object*, m_parent)
   END_OBJECT()
   REGISTER_RTTI( RegularObject )
   // -------------------------------------------------------------------------

   class PropertiesViewMock : public TPropertiesView<PropertiesViewMock>
   {
   private:
      class IntEditorMock : public TPropertyEditor<PropertiesViewMock>
      {
      public:
         IntEditorMock( TEditableProperty< int >* property )
         {
            delete property;
         }

         void initialize(PropertiesViewMock& view) {}

         void deinitialize(PropertiesViewMock& view) {}
      };

   public:
      PropertiesViewMock()
      {
         associate< int, IntEditorMock >();
      }
   };

   // -------------------------------------------------------------------------

   class SerializerImplMock : public SerializerImpl
   {
   private:
      std::vector<byte>& m_storage;

   public:
      SerializerImplMock(std::vector<byte>& storage)
         : m_storage(storage)
      {}

      void write(byte* buf, unsigned int bytesCount)
      {
         for (unsigned int i = 0; i < bytesCount; ++i)
         {
            m_storage.push_back(buf[i]);
         }
      }

      std::size_t read(byte* buf, unsigned int bytesCount) 
      {
         unsigned int bytesLeft = m_storage.size();
         unsigned int bytesToRead = (bytesLeft < bytesCount) ? bytesLeft : bytesCount;

         for (unsigned int i = 0; i < bytesToRead; ++i)
         {
            buf[i] = m_storage[0];
            m_storage.erase(m_storage.begin());
         }

         return bytesToRead;
      }
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(Object, properties)
{
   BaseObject baseObj;
   DerivedObject derivedObj;
   PropertiesViewMock propertiesView;

   baseObj.viewProperties(propertiesView);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, propertiesView.getEditorsCount());

   derivedObj.viewProperties(propertiesView);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, propertiesView.getEditorsCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Object, serialization)
{
   ResourcesManager& mgr = ResourcesManager::getInstance();
   mgr.reset();

   BaseObject baseObj(1);
   DerivedObject derivedObj(3, 2);

   std::vector<byte> storage;
   Saver saver(new SerializerImplMock(storage));
   Loader loader(new SerializerImplMock(storage));

   saver.save( baseObj );
   BaseObject& restoredObj1 = dynamic_cast< BaseObject& >( loader.load( mgr ) );
   CPPUNIT_ASSERT_EQUAL(1, restoredObj1.m_val);

   storage.clear();

   saver.save( derivedObj );
   BaseObject& restoredObj2 = dynamic_cast< BaseObject& >( loader.load( mgr ) );
   CPPUNIT_ASSERT_EQUAL(2, restoredObj2.m_val);
   CPPUNIT_ASSERT_EQUAL(3, (dynamic_cast< DerivedObject& >( restoredObj2 )).m_val);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Object, dependencies)
{
   ResourcesManager& mgr = ResourcesManager::getInstance();
   mgr.reset();

   // case 1: dependency not set
   {
      DependentObject depObj;

      std::vector<byte> storage;
      Saver saver(new SerializerImplMock(storage));
      Loader loader(new SerializerImplMock(storage));

      saver.save(depObj);
      DependentObject& restoredObj = dynamic_cast< DependentObject& >( loader.load( mgr ) );
      CPPUNIT_ASSERT(restoredObj.m_obj == NULL);
   }

   // case 2: dependency set
   {
      BaseObject* baseObj = new BaseObject(5);
      mgr.addResource( baseObj );
      DependentObject depObj( baseObj );

      std::vector<byte> storage;
      Saver saver(new SerializerImplMock(storage));
      Loader loader(new SerializerImplMock(storage));

      saver.save( depObj );
      DependentObject& restoredObj = dynamic_cast< DependentObject& >( loader.load( mgr ) );
      CPPUNIT_ASSERT(restoredObj.m_obj != NULL);
      CPPUNIT_ASSERT_EQUAL(5, restoredObj.m_obj->m_val);
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST( Object, resourcesTreatedAsSingletons )
{
   BaseObject* baseObj = new BaseObject(5);
   DependentObject depObj1( baseObj );
   DependentObject depObj2( baseObj );

   std::vector<byte> storage;
   ResourcesManager& mgr = ResourcesManager::getInstance();
   mgr.reset();

   mgr.addResource( baseObj );

   Saver saver(new SerializerImplMock(storage));
   saver.save( depObj1 );
   saver.save( depObj2 );

   Loader loader(new SerializerImplMock(storage));
   DependentObject& restoredObj1 = dynamic_cast< DependentObject& >( loader.load( mgr ) );
   DependentObject& restoredObj2 = dynamic_cast< DependentObject& >( loader.load( mgr ) );
   CPPUNIT_ASSERT(&restoredObj1 != &restoredObj2);
   CPPUNIT_ASSERT(restoredObj1.m_obj == restoredObj2.m_obj);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Object, inheritance)
{
   ResourcesManager& mgr = ResourcesManager::getInstance();
   mgr.reset();

   ImplementingClass obj;
   obj.setBaseVal(1);
   obj.setVal(2);

   std::vector<byte> storage;
   Saver saver(new SerializerImplMock(storage));
   Loader loader(new SerializerImplMock(storage));

   saver.save(obj);
   AbstractClass& restoredObj = dynamic_cast< AbstractClass& >( loader.load( mgr ) );
   CPPUNIT_ASSERT_EQUAL(1, restoredObj.getBaseVal());
   CPPUNIT_ASSERT_EQUAL(2, restoredObj.getVal());
}

///////////////////////////////////////////////////////////////////////////////
