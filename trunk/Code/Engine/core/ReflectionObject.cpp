#include "core/ReflectionObject.h"
#include "core/ReflectionType.h"
#include "core/ReflectionPropertiesView.h"
#include "core/ReflectionProperties.h"


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
   const SerializableReflectionType& thisType = getVirtualRTTI();

   ReflectionProperties* properties = new ReflectionProperties( *this, thisType.m_name );
   thisType.collectProperties( this, *properties );

   view.set( properties );
}

///////////////////////////////////////////////////////////////////////////////
