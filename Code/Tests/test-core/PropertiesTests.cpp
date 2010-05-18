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
      EditorMock( TEditableProperty< T >* property ) 
      {
         delete property;
      }

      void initialize( PropertiesViewMock& view ) {}

      void deinitialize( PropertiesViewMock& view ) {}
   };

   // -------------------------------------------------------------------------

   class SerializableMock : public Serializable
   {
      DECLARE_RTTI_CLASS

   public:
      void onSave(Serializer& serializer) {}
      void onLoad(Serializer& serializer) {}
      
      int getClassID() const { return getRTTIClass().getHandle(); }
   };
   BEGIN_RTTI( SerializableMock )
   END_RTTI

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(Properties, settingProperties)
{
   D3DXVECTOR3 position(0, 0, 0);
   D3DXVECTOR3 speed(0, 0, 0);

   Properties properties("");
   properties.add<D3DXVECTOR3>( position, "Pos" );
   properties.add<D3DXVECTOR3>( speed, "Speed" );

   CPPUNIT_ASSERT_EQUAL(true, properties.has("Pos"));
   CPPUNIT_ASSERT_EQUAL(true, properties.has("Speed"));
   CPPUNIT_ASSERT_EQUAL(false, properties.has("Orientation"));
}

///////////////////////////////////////////////////////////////////////////////

TEST(TPropertiesView, editors)
{
   D3DXVECTOR3 position(0, 0, 0);
   D3DXVECTOR3 speed(0, 0, 0);

   Properties properties("");
   properties.add<D3DXVECTOR3>( position, "position" ).setParams( "position", "", true );
   properties.add<D3DXVECTOR3>( speed, "speed" ).setParams( "speed", "", true );

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
   factory.mockAssociatePtr< SerializableMock*, EditorMock< SerializableMock* > >();
   
   int intVal;
   float floatVal;
   SerializableMock* ptr = new SerializableMock();
   TProperty<int> intProperty(&intVal);
   TProperty<float> floatProperty(&floatVal);
   TProperty<SerializableMock*> floatPtrProperty(&ptr);

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
   CPPUNIT_ASSERT(NULL != dynamic_cast< EditorMock< SerializableMock* >* > (editor));
   delete editor;
   delete ptr;
}

///////////////////////////////////////////////////////////////////////////////
