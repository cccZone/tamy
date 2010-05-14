#include "core-TestFramework\TestFramework.h"
#include "core\Property.h"
#include "core\Properties.h"
#include "core\PropertyEditor.h"
#include "core\PropertiesView.h"
#include "core\MatrixWriter.h"
#include "core\Serializable.h"
#include "core\Class.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class PropertiesViewMock : public TPropertiesView< PropertiesViewMock >
   {
   public:
      template< typename T, typename E >
      void mockAssociate()
      {
         associate< T, E >();
      }

      template< typename T, typename E >
      void mockAssociatePtr()
      {
         associatePtr< T, E >();
      }

      PropertyEditor* mockCreate( Property& property )
      {
         return create( property );
      }
   };

   // -------------------------------------------------------------------------

   template<typename T>
   class EditorMock : public TPropertyEditor< PropertiesViewMock >
   {
   public:
      EditorMock( T& val, const std::string& label ) {}

      void initialize( PropertiesViewMock& view ) {}

      void deinitialize( PropertiesViewMock& view ) {}
   };

   // -------------------------------------------------------------------------

   template<typename T>
   class PtrEditorMock : public TPropertyEditor< PropertiesViewMock >
   {
   public:
      PtrEditorMock( T*& val, const Class& acceptableType, const std::string& label ) {}

      void initialize( PropertiesViewMock& view ) {}

      void deinitialize( PropertiesViewMock& view ) {}
   };

   // -------------------------------------------------------------------------

   class SerializableMock : public Serializable
   {
   private:
      Class m_handle;

   public:
      SerializableMock()
      {
         ClassTemplate& temp = getClassesRegistry().defineClass< SerializableMock >();
         m_handle = Class( temp );
      }

      void onSave(Serializer& serializer) {}
      void onLoad(Serializer& serializer) {}
      
      int getClassID() const { return m_handle.getHandle(); }

      Class getVirtualClass() const { return m_handle; }
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(Properties, settingProperties)
{
   D3DXVECTOR3 position(0, 0, 0);
   D3DXVECTOR3 speed(0, 0, 0);

   Properties properties("");
   properties.add<D3DXVECTOR3>("Pos", "PosLabel", position);
   properties.add<D3DXVECTOR3>("Speed", "SpeedLabel", speed);

   CPPUNIT_ASSERT_EQUAL(true, properties.has("Pos"));
   CPPUNIT_ASSERT_EQUAL(true, properties.has("Speed"));
   CPPUNIT_ASSERT_EQUAL(false, properties.has("Orientation"));
}

///////////////////////////////////////////////////////////////////////////////

TEST(Properties, changingPropertyValue)
{
   D3DXVECTOR3 position(0, 0, 0);

   Properties properties("");
   properties.add<D3DXVECTOR3>("Pos", "PosLabel", position);

   D3DXVECTOR3 newPosition(10, 20, 30);
   properties.get<D3DXVECTOR3>("Pos").set(D3DXVECTOR3(10, 20, 30));
   CPPUNIT_ASSERT_EQUAL(newPosition, position);
}

///////////////////////////////////////////////////////////////////////////////

TEST(TPropertiesView, editors)
{
   D3DXVECTOR3 position(0, 0, 0);
   D3DXVECTOR3 speed(0, 0, 0);

   Properties properties("");
   properties.add<D3DXVECTOR3>("position", "positionLabel", position);
   properties.add<D3DXVECTOR3>("speed", "speedLabel", speed);

   PropertiesViewMock view;
   view.mockAssociate< D3DXVECTOR3, EditorMock< D3DXVECTOR3 > >();
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, view.getEditorsCount());

   view.set(properties);

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, view.getEditorsCount());

   view.reset();

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, view.getEditorsCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Properties, dynamicEditorCreation)
{
   PropertiesViewMock factory;
   factory.mockAssociate< int, EditorMock< int > >();
   factory.mockAssociate< float, EditorMock< float > >();
   factory.mockAssociatePtr< SerializableMock, PtrEditorMock< SerializableMock > >();
   
   int intVal;
   float floatVal;
   SerializableMock* ptr = new SerializableMock();
   TProperty<int> intProperty(&intVal, "intVal", "intVal");
   TProperty<float> floatProperty(&floatVal, "floatVal", "floatVal");
   TProperty<SerializableMock*> floatPtrProperty(&ptr, "ptrProperty", "ptrProperty");

   PropertyEditor* editor = factory.mockCreate(intProperty);
   CPPUNIT_ASSERT(NULL != editor);
   CPPUNIT_ASSERT(NULL != dynamic_cast<EditorMock<int>*> (editor));
   delete editor;

   editor = factory.mockCreate(floatProperty);
   CPPUNIT_ASSERT(NULL != editor);
   CPPUNIT_ASSERT(NULL != dynamic_cast<EditorMock<float>*> (editor));
   delete editor;

   editor = factory.mockCreate(floatPtrProperty);
   CPPUNIT_ASSERT(NULL != editor);
   CPPUNIT_ASSERT(NULL != dynamic_cast< PtrEditorMock< SerializableMock >* > (editor));
   delete editor;
   delete ptr;
}

///////////////////////////////////////////////////////////////////////////////
