#include "core/ReflectionObject.h"
#include "core/ReflectionType.h"
#include "core/ReflectionPropertiesView.h"
#include "core/ReflectionProperty.h"


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
