#include "core-TestFramework\TestFramework.h"
#include "core\ReflectionType.h"
#include "core\ReflectionObject.h"
#include "core\ReflectionObjectChangeListener.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   struct ReflectionObjectMock_ROT : public ReflectionObject
   {
      DECLARE_STRUCT()

      int      m_val; // static data

      // runtime data
      bool     m_preNotificationReceived;
      bool     m_postNotificationReceived;

      ReflectionObjectMock_ROT( int val = 0 ) : m_val( val ), m_preNotificationReceived( 0 ), m_postNotificationReceived( 0 ) {}

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
   BEGIN_OBJECT( ReflectionObjectMock_ROT );
      PARENT( ReflectionObject );
      PROPERTY_EDIT( "m_val", int, m_val );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct MockObjectListener : public ReflectionObjectChangeListener
   {
      bool     m_notificationReceived;

      MockObjectListener() : m_notificationReceived( false ) {}

      void onPropertyChanged( ReflectionProperty& property )
      {
         m_notificationReceived = ( property.getName() == "m_val" );
      }
   };

}

///////////////////////////////////////////////////////////////////////////////

DEFINE_TYPE_ID( ReflectionObjectMock_ROT );

///////////////////////////////////////////////////////////////////////////////

TEST( ReflectionObject, propertyChangesObservation )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< ReflectionObject>( "ReflectionObject", NULL );
   typesRegistry.addSerializableType< ReflectionObjectMock_ROT >( "ReflectionObjectMock", new TSerializableTypeInstantiator< ReflectionObjectMock_ROT >() );

   ReflectionObjectMock_ROT object;
   MockObjectListener listener;
   object.attachListener( listener );

   object.notifyPropertyChange( "m_val" );
   CPPUNIT_ASSERT( false == object.m_preNotificationReceived );
   CPPUNIT_ASSERT( true == object.m_postNotificationReceived );
   CPPUNIT_ASSERT( true == listener.m_notificationReceived );

   // cleanup
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////
