#ifndef _REFLECTION_SAVER_H
#error "This file can only be included in ReflectionSaver.h"
#else

#include "core/ReflectionType.h"
#include "core/OutStream.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ReflectionSaver::save( const T& object )
{
   // Check if the object isn't already stored somewhere in the dependencies map.
   // If it is, there's no point in serializing it again.
   uint count = m_dependencies.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_dependencies[i] == &object )
      {
         // it was already serialized - just add its dependency index to the saved objects list
         m_serializedObjectsIndices.push_back( i );
         return;
      }
   }

   // Create a map of all pointers we're about to serialize.
   // In other words - map the dependencies the saved object has

   // ...and the very first dependency is our object itself - memorize its index so that we know
   // which object on the long list of serialized objects was the serialized one
   m_serializedObjectsIndices.push_back( m_dependencies.size() );
   addDependency( object );

   // map its dependencies
   const SerializableReflectionType& typeInfo = object.getVirtualRTTI();
   typeInfo.mapDependencies( object, *this );

}

///////////////////////////////////////////////////////////////////////////////

uint ReflectionSaver::findDependency( const ReflectionObject& dependency ) const
{
   uint dependenciesCount = m_dependencies.size();
   for ( uint i = 0; i < dependenciesCount; ++i )
   {
      if ( m_dependencies[i] == &dependency )
      {
         // found it
         return i + 1;
      }
   }

   // we didn't map such a dependency - how did it got queried for then?
   ASSERT_MSG( false, "Unmapped dependency queried - check the dependency mapping related code" );
   return 0;
}

///////////////////////////////////////////////////////////////////////////////


#endif // _REFLECTION_SAVER_H
