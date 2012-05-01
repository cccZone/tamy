#ifndef _REFLECTION_LOADER_H
#error "This file can only be included in ReflectionLoader.h"
#else

#include "core/ReflectionType.h"
#include "core/InStream.h"


///////////////////////////////////////////////////////////////////////////////

template< typename T >
T* ReflectionLoader::load()
{
   // perform a lazy load
   if ( m_loadedObjects.empty() )
   {
      initialize();
   }

   // get the first object from the list, or return NULL if it's empty
   if ( m_loadedObjects.empty() )
   {
      return NULL;
   }
   else
   {
      ReflectionObject* obj = m_loadedObjects.front();
      m_loadedObjects.pop_front();

      // notify the object that it's been successfully loaded
      obj->onObjectLoaded();

      return static_cast< T* >( obj );
   }
}

///////////////////////////////////////////////////////////////////////////////

ReflectionObject* ReflectionLoader::findDependency( uint dependencyIdx ) const
{
   if ( dependencyIdx == 0 )
   {
      // this is a NULL reference
      return NULL;
   }
   else
   {
      --dependencyIdx;
      return ( dependencyIdx < m_dependencies.size() ) ? m_dependencies[dependencyIdx] : NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_LOADER_H
