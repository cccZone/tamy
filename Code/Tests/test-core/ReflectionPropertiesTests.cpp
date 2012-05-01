#include "core-TestFramework\TestFramework.h"
#include "core\ReflectionProperty.h"
#include "core\ReflectionType.h"
#include "core\ReflectionProperties.h"
#include "core\ReflectionPropertyEditor.h"
#include "core\ReflectionPropertiesView.h"
#include "core\MatrixWriter.h"
#include "core\ReflectionObject.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class PropertiesViewMock : public TReflectionPropertiesView< PropertiesViewMock >
   {
   public:
      ReflectionPropertyEditor* mockCreate( ReflectionProperty& property )
      {
         return create( property );
      }
   };

   // -------------------------------------------------------------------------

   template<typename T>
   class EditorMock : public TReflectionPropertyEditor< PropertiesViewMock >
   {
   private:
      TEditableReflectionProperty< T >*         m_property;

   public:
      EditorMock( TEditableReflectionProperty< T >* property ) 
         : m_property( property )
      {
      }

      ~EditorMock()
      {
         delete m_property;
      }

      void initialize( PropertiesViewMock& view ) {}

      void deinitialize( PropertiesViewMock& view ) {}

      void setValue( T val )
      {
         m_property->set( val );
      }
   };

   // -------------------------------------------------------------------------

   struct ReflectionObjectMock : public ReflectionObject
   {
      DECLARE_STRUCT()

      int      m_val; // static data

      // runtime data
      bool     m_preNotificationReceived;
      bool     m_postNotificationReceived;

      ReflectionObjectMock() : m_val( 0 ), m_preNotificationReceived( 0 ), m_postNotificationReceived( 0 ) {}

      void onPrePropertyChanged( ReflectionProperty& property )
      {
         m_preNotificationReceived = true;
      }

      void onPropertyChanged( ReflectionProperty& property )
      {
         m_postNotificationReceived = true;
      }

      bool wasNotified() const 
      {
         return m_preNotificationReceived && m_postNotificationReceived;
      }
   };
   BEGIN_OBJECT( ReflectionObjectMock );
      PROPERTY_EDIT( "m_val", int, m_val );
   END_OBJECT();

} // anonymous

///////////////////////////////////////////////////////////////////////////////

DEFINE_TYPE_ID( ReflectionObjectMock )

///////////////////////////////////////////////////////////////////////////////

TEST( ReflectionProperties, settingProperties )
{
   D3DXVECTOR3 position(0, 0, 0);
   D3DXVECTOR3 speed(0, 0, 0);

   ReflectionObjectMock object;
   ReflectionProperties properties( object, "");
   properties.add<D3DXVECTOR3>( position, "Pos" );
   properties.add<D3DXVECTOR3>( speed, "Speed" );

   CPPUNIT_ASSERT_EQUAL(true, properties.has("Pos"));
   CPPUNIT_ASSERT_EQUAL(true, properties.has("Speed"));
   CPPUNIT_ASSERT_EQUAL(false, properties.has("Orientation"));
}

///////////////////////////////////////////////////////////////////////////////

TEST(TReflectionPropertiesView, editors)
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addExternalType< D3DXVECTOR3 >( "D3DXVECTOR3" );

   D3DXVECTOR3 position(0, 0, 0);
   D3DXVECTOR3 speed(0, 0, 0);

   ReflectionObjectMock object;
   ReflectionProperties* properties = new ReflectionProperties( object, "" );
   properties->add<D3DXVECTOR3>( position, "position" ).setParams( "position", "", true, true );
   properties->add<D3DXVECTOR3>( speed, "speed" ).setParams( "speed", "", true, true );

   PropertiesViewMock view;
   view.associate< D3DXVECTOR3, EditorMock< D3DXVECTOR3 > >();
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, view.getEditorsCount());

   view.set( properties );

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, view.getEditorsCount());

   view.reset();

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, view.getEditorsCount());

   // cleanup
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( ReflectionProperties, dynamicEditorCreation )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addExternalType< int >( "int" );
   typesRegistry.addExternalType< float >( "float" );
   typesRegistry.addSerializableType< ReflectionObjectMock >( "ReflectionObjectMock", new TSerializableTypeInstantiator< ReflectionObjectMock >() );

   PropertiesViewMock factory;
   factory.associate< int, EditorMock< int > >();
   factory.associate< float, EditorMock< float > >();
   factory.associateAbstract< ReflectionObjectMock*, EditorMock< ReflectionObjectMock* > >();

   int intVal;
   float floatVal;
   ReflectionObjectMock hostObject;
   ReflectionObjectMock* ptr = new ReflectionObjectMock();
   TReflectionProperty<int> intProperty( &hostObject, &intVal );
   TReflectionProperty<float> floatProperty( &hostObject, &floatVal );
   TReflectionProperty< ReflectionObjectMock* > objProperty( &hostObject, &ptr );

   ReflectionPropertyEditor* editor = factory.mockCreate(intProperty);
   CPPUNIT_ASSERT(NULL != editor);
   CPPUNIT_ASSERT(NULL != dynamic_cast<EditorMock<int>*> (editor));
   delete editor;

   editor = factory.mockCreate(floatProperty);
   CPPUNIT_ASSERT(NULL != editor);
   CPPUNIT_ASSERT(NULL != dynamic_cast<EditorMock<float>*> (editor));
   delete editor;

   editor = factory.mockCreate(objProperty);
   CPPUNIT_ASSERT(NULL != editor);
   CPPUNIT_ASSERT(NULL != dynamic_cast< EditorMock< ReflectionObjectMock* >* > (editor));
   delete editor;
   delete ptr;

   // cleanup
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( ReflectionProperties, access )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addExternalType< int >( "int" );
   typesRegistry.addSerializableType< ReflectionObjectMock >( "ReflectionObjectMock", new TSerializableTypeInstantiator< ReflectionObjectMock >() );

   // create the properties viewer
   PropertiesViewMock propertiesView;
   propertiesView.associate< int, EditorMock< int > >();

   // create an object and view its properties
   ReflectionObjectMock obj;

   CPPUNIT_ASSERT_EQUAL( (uint)0, propertiesView.getEditorsCount() );

   obj.viewProperties( propertiesView );

   CPPUNIT_ASSERT_EQUAL( (uint)1, propertiesView.getEditorsCount() );

   // cleanup
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( ReflectionProperties, observation )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addExternalType< int >( "int" );
   typesRegistry.addSerializableType< ReflectionObjectMock >( "ReflectionObjectMock", new TSerializableTypeInstantiator< ReflectionObjectMock >() );

   // create the properties viewer
   PropertiesViewMock propertiesView;
   propertiesView.associate< int, EditorMock< int > >();

   // create an object and view its properties
   ReflectionObjectMock obj;
   obj.viewProperties( propertiesView );

   EditorMock< int >& editor = dynamic_cast< EditorMock< int >& >( propertiesView.getEditor(0) );
   editor.setValue( 12 );

   CPPUNIT_ASSERT( obj.wasNotified() );

   // cleanup
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( ReflectionProperties, vectorsOfObjects )
{
   // TODO: vectorsOfObjects
}

///////////////////////////////////////////////////////////////////////////////
