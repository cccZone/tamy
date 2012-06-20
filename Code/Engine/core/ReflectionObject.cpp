#include "core/ReflectionObject.h"
#include "core/ReflectionType.h"
#include "core/ReflectionPropertiesView.h"
#include "core/ReflectionProperty.h"
#include "core/ReflectionObjectChangeListener.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( ReflectionObject )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

bool ReflectionObject::isExactlyA( const SerializableReflectionType& referenceType ) const
{
   const SerializableReflectionType& thisType = getVirtualRTTI();
   return thisType.isExactlyA( referenceType );
}

///////////////////////////////////////////////////////////////////////////////

bool ReflectionObject::isA( const SerializableReflectionType& referenceType ) const
{
   const SerializableReflectionType& thisType = getVirtualRTTI();
   return thisType.isA( referenceType );
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionObject::viewProperties( ReflectionPropertiesView& view )
{
   view.set( *this );
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionObject::attachListener( ReflectionObjectChangeListener& listener )
{
   m_listener.push_back( &listener );
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionObject::detachListener( ReflectionObjectChangeListener& listener )
{
   uint idx = m_listener.find( &listener );
   if ( idx < EOA )
   {
      m_listener.remove( idx );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionObject::notifyPropertyChange( const std::string& propertyName )
{
   // find the field in this or the parent types
   ReflectionTypeComponent* propertyField = NULL;
   {
      std::vector< const SerializableReflectionType* > types;
      types.push_back( &getVirtualRTTI() );

      while( !types.empty() )
      {
         const SerializableReflectionType* analyzedType = types.back();
         types.pop_back();

         propertyField = analyzedType->findMemberField( propertyName );
         if ( propertyField != NULL )
         {
            // found it !!!
            break;
         }

         // collect parent types for further analysis
         analyzedType->collectParents( types );
      }
   }

   if ( !propertyField )
   {
      // this field doesn't exist
      return;
   }

   ReflectionProperty* property = propertyField->createProperty( this );

   // notify the object itself
   onPropertyChanged( *property );
   
   // notify the listeners
   uint listenersCount = m_listener.size();
   for ( uint i = 0; i < listenersCount; ++i )
   {
      m_listener[i]->onPropertyChanged( *property );
   }

   // cleanup
   delete property;
}

///////////////////////////////////////////////////////////////////////////////
