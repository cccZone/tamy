#include "core-TestFramework\TestFramework.h"
#include "core\Object.h"
#include "core\Property.h"
#include "core\PropertiesView.h"
#include "core\PropertyEditor.h"
#include "core\Serializer.h"
#include "core\SerializerImpl.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   struct BaseObject : public Object
   {
      DECLARE_STRUCT(BaseObject)

      int   m_val;

      BaseObject(int val = -1) : m_val(val) {}
      virtual ~BaseObject() {}
   };
   BEGIN_OBJECT(BaseObject, Object)
      PROPERTY("some label", int, m_val)
   END_OBJECT()

   // -------------------------------------------------------------------------

   struct DerivedObject : public BaseObject
   {
      DECLARE_STRUCT(DerivedObject)

      int m_val;

      DerivedObject(int val = -2, int baseVal = -1) : BaseObject(baseVal), m_val(val) {}
   };
   BEGIN_OBJECT(DerivedObject, BaseObject)
      PROPERTY("some label", int, m_val)
   END_OBJECT()

   // -------------------------------------------------------------------------

   struct DependentObject : public Object
   {
      DECLARE_STRUCT(DependentObject)

      BaseObject* m_obj;

      DependentObject(BaseObject* obj = NULL) : m_obj(obj) {}
   };
   BEGIN_OBJECT(DependentObject, Object)
      PROPERTY("some label", BaseObject*, m_obj)
   END_OBJECT()

   // -------------------------------------------------------------------------

   class AbstractClass : public Object
   {
      DECLARE_ABSTRACT_CLASS(AbstractClass)

   private:
      int m_baseVal;

   public:
      virtual ~AbstractClass() {}

      void setBaseVal(int val) { m_baseVal = val; }
      int getBaseVal() const { return m_baseVal; }

      virtual void setVal(int val) = 0;
      virtual int getVal() const = 0;

   protected:
      AbstractClass() : m_baseVal(-1) {}
   };
   BEGIN_ABSTRACT_OBJECT(AbstractClass, Object)
      PROPERTY("baseVal", int, m_baseVal)
   END_OBJECT()

   // -------------------------------------------------------------------------

   class ImplementingClass : public AbstractClass
   {
      DECLARE_CLASS(ImplementingClass)

   private:
      int m_val;

   public:
      ImplementingClass() : m_val(-1) {}

      void setVal(int val) { m_val = val; }
      int getVal() const { return m_val; }
   };
   BEGIN_OBJECT(ImplementingClass, AbstractClass)
      PROPERTY("val", int, m_val)
   END_OBJECT()

   // -------------------------------------------------------------------------

   class HierachicalClass : public Object
   {
      DECLARE_CLASS(HierachicalClass)

   public:
      HierachicalClass*                m_parent;
      std::vector< HierachicalClass* > m_children;

   public:
      HierachicalClass() : m_parent(NULL) {}
      ~HierachicalClass() 
      {
         unsigned int count = m_children.size();
         for ( unsigned int i = 0; i < count; ++i )
         {
            delete m_children[i];
         }
         m_children.clear();
      }


      void addChild( HierachicalClass* child )
      {
         if ( !child )
         {
            return;
         }
         child->m_parent = this;
         m_children.push_back( child );
      }

   };
   BEGIN_OBJECT(HierachicalClass, Object)
      PROPERTY("parent", HierachicalClass*, m_parent)
      PROPERTY("children", std::vector< HierachicalClass* >, m_children)
   END_OBJECT()


   // -------------------------------------------------------------------------

   class PropertiesViewMock : public TPropertiesView<PropertiesViewMock>
   {
   private:
      class IntEditorMock : public TPropertyEditor<PropertiesViewMock>
      {
      public:
         IntEditorMock(TProperty<int>& property)
         {
         }

         void initialize(PropertiesViewMock& view) {}

         void deinitialize(PropertiesViewMock& view) {}
      };

   public:
      PropertiesViewMock()
      {
         associate<TProperty<int>, IntEditorMock>();
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
   BaseObject baseObj(1);

   DerivedObject derivedObj(3, 2);

   std::vector<byte> storage;
   Saver saver(new SerializerImplMock(storage));
   Loader loader(new SerializerImplMock(storage));

   saver.save(baseObj);
   BaseObject* restoredObj = loader.load< BaseObject >();
   CPPUNIT_ASSERT_EQUAL(1, restoredObj->m_val);
   delete restoredObj;

   storage.clear();

   saver.save(derivedObj);
   restoredObj = loader.load< DerivedObject >();

   CPPUNIT_ASSERT_EQUAL(2, restoredObj->m_val);
   CPPUNIT_ASSERT_EQUAL(3, ((DerivedObject*)(restoredObj))->m_val);
   delete restoredObj;
}

///////////////////////////////////////////////////////////////////////////////

TEST(Object, dependencies)
{
   // case 1: dependency not set
   {
      DependentObject depObj;

      std::vector<byte> storage;
      Saver saver(new SerializerImplMock(storage));
      Loader loader(new SerializerImplMock(storage));

      saver.save(depObj);
      DependentObject* restoredObj = loader.load< DependentObject >();

      CPPUNIT_ASSERT(restoredObj->m_obj == NULL);
      delete restoredObj;
   }

   // case 2: dependency set
   {
      BaseObject baseObj(5);
      DependentObject depObj(&baseObj);

      std::vector<byte> storage;
      Saver saver(new SerializerImplMock(storage));
      Loader loader(new SerializerImplMock(storage));

      saver.save( depObj );
      DependentObject* restoredObj = loader.load< DependentObject >();
      CPPUNIT_ASSERT(restoredObj->m_obj != NULL);
      CPPUNIT_ASSERT_EQUAL(5, restoredObj->m_obj->m_val);
      delete restoredObj->m_obj;
      delete restoredObj;
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST( Object, dependenciesMapping )
{
   HierachicalClass parent;
   parent.addChild( new HierachicalClass() );

   std::vector<byte> storage;
   Saver saver( new SerializerImplMock( storage ) );
   Loader loader( new SerializerImplMock( storage ) );

   saver.save( parent );
   HierachicalClass* restoredParent = loader.load< HierachicalClass >();

   CPPUNIT_ASSERT( restoredParent != NULL );
   CPPUNIT_ASSERT( restoredParent->m_parent == NULL );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)1, restoredParent->m_children.size() );

   HierachicalClass* restoredChild = restoredParent->m_children[0];
   CPPUNIT_ASSERT( restoredChild != NULL );
   CPPUNIT_ASSERT( restoredChild->m_parent == restoredParent );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)0, restoredChild->m_children.size() );

   delete restoredParent;
}

///////////////////////////////////////////////////////////////////////////////

TEST(Object, inheritance)
{
   ImplementingClass obj;
   obj.setBaseVal(1);
   obj.setVal(2);

   std::vector<byte> storage;
   Saver saver(new SerializerImplMock(storage));
   Loader loader(new SerializerImplMock(storage));

   saver.save(obj);
   AbstractClass* restoredObj = loader.load< AbstractClass >();
   CPPUNIT_ASSERT_EQUAL(1, restoredObj->getBaseVal());
   CPPUNIT_ASSERT_EQUAL(2, restoredObj->getVal());
   delete restoredObj;
}

///////////////////////////////////////////////////////////////////////////////
