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
