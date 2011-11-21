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

template< typename RESOURCE_TYPE >
RESOURCE_TYPE& ResourcesManager::create( const FilePath& name )
{
   FilePath nameWithCorrectExtension;
   name.changeFileExtension( RESOURCE_TYPE::getExtension(), nameWithCorrectExtension ) ;
   Resource& res = create( nameWithCorrectExtension );

   return *DynamicCast< RESOURCE_TYPE >( &res );
}

///////////////////////////////////////////////////////////////////////////////

template< typename RESOURCE_TYPE >
RESOURCE_TYPE* ResourcesManager::findResource( const FilePath& name )
{
   FilePath nameWithCorrectExtension;
   name.changeFileExtension( RESOURCE_TYPE::getExtension(), nameWithCorrectExtension ) ;
   Resource* res = findResource( nameWithCorrectExtension );
   if ( res )
   {
      return DynamicCast< RESOURCE_TYPE >( res );
   }
   else
   {
      return NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCES_MANAGER_H
