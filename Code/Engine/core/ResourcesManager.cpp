#include "core\ResourcesManager.h"
#include "core\IProgressObserver.h"
#include "core\ReflectionSerializationUtil.h"
#include "core\Profiler.h"


///////////////////////////////////////////////////////////////////////////////

ResourcesManager ResourcesManager::s_theInstance;

///////////////////////////////////////////////////////////////////////////////

ResourcesManager::ResourcesManager()
: m_filesystem( new Filesystem() )
, m_progressObserverCreator( NULL )
{
   m_filesystem->attach( *this );
}

///////////////////////////////////////////////////////////////////////////////

ResourcesManager::~ResourcesManager()
{
   // delete the loaders
   for ( ResourceImportersMap::iterator itArr = m_importers.begin(); itArr != m_importers.end(); ++itArr )
   {
      ImportersArr* arr = itArr->second;
      for ( ImportersArr::iterator it = arr->begin(); it != arr->end(); ++it )
      {
         delete *it;
      }
      delete arr;
   }
   m_importers.clear();

   delete m_progressObserverCreator; m_progressObserverCreator = NULL;

   m_filesystem->detach( *this );
   delete m_filesystem; m_filesystem = NULL;

   reset();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::reset()
{
   unsigned int componentsCount = getComponentsCount();

   for ( ResourcesMap::iterator it = m_resources.begin(); it != m_resources.end(); ++it )
   {
      Resource* resource = it->second;
      if ( resource )
      {
         // inform the resource about the components being removed
         for ( unsigned int compIdx = 0; compIdx < componentsCount; ++compIdx )
         {
            Component< ResourcesManager >* comp = getComponent( compIdx );
            resource->onComponentRemoved( *comp );
         }
      }

      resource->resetResourcesManager();
      resource->removeReference();
   }
   m_resources.clear();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::setFilesystem( Filesystem* filesystem )
{
   if (filesystem == NULL)
   {
      ASSERT_MSG( false,  "NULL pointer instead a Filesystem instance" );
      return;
   }

   // we need to delete all resources along with changing the filesystem
   reset();

   if ( m_filesystem )
   {
      m_filesystem->detach( *this );
   }
   delete m_filesystem;
   m_filesystem = filesystem;
   m_filesystem->attach( *this );
}

///////////////////////////////////////////////////////////////////////////////

bool ResourcesManager::addResource( Resource* resource )
{
   if ( resource == NULL )
   {
      return false;
   }

   if ( !registerNewResource( resource ) )
   {
      return false;
   }

   resource->finalizeResourceLoading();
   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool ResourcesManager::registerNewResource( Resource* resource )
{
   if ( resource == NULL )
   {
      return false;
   }

   if ( resource->isManaged() )
   {
      return false;
   }

   // correct the resource's name
   FilePath correctResourcePath;
   resource->getFilePath().changeFileExtension( resource->getVirtualExtension(), correctResourcePath );

   ResourcesMap::iterator it = m_resources.find( correctResourcePath );
   if ( it != m_resources.end() )
   {
      // a resource by this name already exists - and we can't replace it, since
      // pointers to it may exist all over the running application
      resource->removeReference();
      return false;
   }

   resource->setFilePath( correctResourcePath );
   m_resources.insert( std::make_pair( correctResourcePath, resource ) );
   resource->setResourcesManager( *this );

   return true;
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::moveResource( Resource* resource, const FilePath& newPath )
{
   if ( resource == NULL )
   {
      ASSERT_MSG( false,  "NULL pointer instead a Resource instance" );
      return;
   }

   if ( resource->isManaged() )
   {
      return;
   }

   // correct the resource's name
   FilePath correctResourcePath;
   newPath.changeFileExtension( resource->getVirtualExtension(), correctResourcePath );

   ResourcesMap::iterator it = m_resources.find( correctResourcePath );
   if ( it == m_resources.end() )
   {
      return;
   }

   resource->setFilePath( correctResourcePath );
   m_resources.erase( it );
   m_resources.insert( std::make_pair( correctResourcePath, resource ) );
}

///////////////////////////////////////////////////////////////////////////////

Resource* ResourcesManager::findResource( const FilePath& name )
{
   PROFILED();

   ResourcesMap::iterator it = m_resources.find( name );
   if ( it != m_resources.end() )
   {
      return it->second;
   }
   else
   {
      return NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::scan( const FilePath& rootDir, FilesystemScanner& scanner, bool recursive ) const
{
   // scan the contents of the filesystem
   m_filesystem->scan( rootDir, scanner, recursive );

   // scan the registered resources
   const std::size_t rootDirNameLen = rootDir.getRelativePath().length();
   for ( ResourcesMap::const_iterator it = m_resources.begin(); it != m_resources.end(); ++it )
   {
      std::size_t rootDirPos = it->first.getRelativePath().find( rootDir );
      if ( rootDirPos != 0 )
      {
         // we only want to consider files located in the root directory
         continue;
      }

      if ( recursive )
      {
         // it's a recursive search - add all files
         scanner.onFile( it->first );
      }
      else
      {
         // it's not a recursive search - add only the files in this directory
         std::size_t subDirCharPos = it->first.getRelativePath().find_first_of( "/\\", rootDirNameLen );
         if ( subDirCharPos == std::string::npos )
         {
            scanner.onFile( it->first );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::remove( const FilePath& path )
{
   m_filesystem->remove( path );
}

///////////////////////////////////////////////////////////////////////////////

Resource* ResourcesManager::create( const FilePath& filePath, bool loadOnly )
{
   PROFILED();

   Resource* res = findResource( filePath );
   if ( res == NULL )
   {
      // the resource doesn't exist yet - try loading it
      res = loadResource( filePath );

      if ( !res && !loadOnly )
      {
         // the resource doesn't exist at all - create a new one
         const SerializableReflectionType* resourceClass = static_cast< const SerializableReflectionType* >( Resource::findResourceClass( filePath.extractExtension() ) );
         if ( resourceClass )
         {
            res = resourceClass->instantiate< Resource >();
         }

         // set the path of the resource and register it with the manager
         if ( res )
         {
            res->setFilePath( filePath );
            addResource( res );

            // last but not least - save the new resource on the hard drive
            res->saveResource();
         }
      }
   }
   
   return res;
}

///////////////////////////////////////////////////////////////////////////////

Resource* ResourcesManager::loadResource( const FilePath& filePath )
{
   PROFILED();

   // initialize the progress observer
   IProgressObserver* progressObserver = createObserver();
   Resource* res = ReflectionSerializationUtil::loadResource( filePath, progressObserver );
   delete progressObserver;

   return res;
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::save( const FilePath& filePath )
{
   PROFILED();

   Resource* resourceToSave = findResource( filePath );
   if ( !resourceToSave )
   {
      // nothing to save
      return;
   }

   IProgressObserver* progressObserver = createObserver();
   ReflectionSerializationUtil::saveResource( resourceToSave, progressObserver );
   delete progressObserver;
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::free( Resource* resource )
{
   if ( resource == NULL )
   {
      return;
   }

   ResourcesMap::iterator it = m_resources.find( resource->getFilePath() );
   if ( it != m_resources.end() && it->second != resource )
   {
      char errMsg[512];
      sprintf_s( errMsg, "Resources discrepancy - two resources share the same name '%s'", resource->getFilePath().getRelativePath().c_str() );
      ASSERT_MSG( false, errMsg );

      // find a matching resource the hard way
      for ( it = m_resources.begin(); it != m_resources.end(); ++it )
      {
         if ( it->second == resource )
         {
            break;
         }
      }
   }

   if ( it != m_resources.end() )
   {
      m_resources.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////

IProgressObserver* ResourcesManager::createObserver() const
{
   if ( m_progressObserverCreator )
   {
      return m_progressObserverCreator->create();
   }
   else
   {
      return new NullProgressObserver();
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::onComponentAdded( Component< ResourcesManager >& component )
{
   for ( ResourcesMap::iterator it = m_resources.begin(); 
      it != m_resources.end(); ++it )
   {
      it->second->onComponentAdded( component );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::onComponentRemoved( Component< ResourcesManager >& component )
{
   for ( ResourcesMap::iterator it = m_resources.begin(); it != m_resources.end(); ++it )
   {
      it->second->onComponentRemoved( component );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::onDirAdded( const FilePath& dir )
{
   // nothing to do here
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::onDirRemoved( const FilePath& dir )
{
   std::vector< FilePath > entriesToRemove;

   // go through the registered resources and see if any of those 
   // still exist - if they don't remove them from the system
   for ( ResourcesMap::iterator it = m_resources.begin(); it != m_resources.end(); ++it )
   {
      const FilePath& path = it->first;
      if ( path.isSubPath( dir ) || path == dir )
      {
         bool doesResourceExist = m_filesystem->doesExist( path );
         if ( !doesResourceExist )
         {
            entriesToRemove.push_back( path );
         }
      }
   }

   // remove the removed resources entries from the map
   unsigned int count = entriesToRemove.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      ResourcesMap::iterator it = m_resources.find( entriesToRemove[i] );
      Resource* res = it->second;
      m_resources.erase( it );

      res->resetResourcesManager();
      res->removeReference();
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::onFileEdited( const FilePath& path )
{
   // nothing to do here
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::onFileRemoved( const FilePath& path )
{
   // check if the removed file was a resource, and if it was - remove it
   ResourcesMap::iterator it = m_resources.find( path );
   if ( it != m_resources.end() )
   {
      Resource* res = it->second;
      m_resources.erase( it );

      res->resetResourcesManager();
      res->removeReference();
   }
}

///////////////////////////////////////////////////////////////////////////////