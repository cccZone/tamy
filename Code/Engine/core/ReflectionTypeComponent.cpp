#include "core/ReflectionTypeComponent.h"


///////////////////////////////////////////////////////////////////////////////

ReflectionTypeComponent::ReflectionTypeComponent( const std::string& memberName )
   : m_memberName( memberName )
   , m_isEditable( false )
   , m_canBeSaved( true )
{
   m_id = generateId( memberName );
}

///////////////////////////////////////////////////////////////////////////////

ReflectionProperty* ReflectionTypeComponent::createProperty( void* object ) const
{
   ReflectionProperty* property = instantiateProperty( object );
   
   if( property != NULL )
   {
      property->setParams( m_memberName, m_label, m_isEditable, m_canBeSaved );
   }

   return property;
}

///////////////////////////////////////////////////////////////////////////////
