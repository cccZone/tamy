#ifndef _RESOURCES_MANAGER_H
#error "This file can only be included from ResourcesManager.h"
#else

#include <stdexcept>
#include "core\Filesystem.h"
#include "core\Resource.h"


///////////////////////////////////////////////////////////////////////////////

unsigned int ResourcesManager::getResourcesCount() const
{
   return m_resources.size();
}

///////////////////////////////////////////////////////////////////////////////

Filesystem& ResourcesManager::getFilesystem()
{
   return *m_filesystem;
}

///////////////////////////////////////////////////////////////////////////////

const Filesystem& ResourcesManager::getFilesystem() const
{
   return *m_filesystem;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ResourcesManager::addLoader( const std::string& extension )
{
   ResourceLoadersMap::iterator it = m_loaders.find( extension );
   if ( it != m_loaders.end() )
   {
      delete it->second;
      it->second = new TResourceLoaderCreator< T >();
   }
   else
   {
      m_loaders.insert( std::make_pair( extension, new TResourceLoaderCreator< T >() ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ResourcesManager::setProgressObserver()
{
   delete m_progressObserverCreator;
   m_progressObserverCreator = new TProgressObserverCreator< T >();
}

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCES_MANAGER_H
