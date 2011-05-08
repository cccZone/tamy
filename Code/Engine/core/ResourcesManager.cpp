#include "core\ResourcesManager.h"
#include "core\ResourceLoader.h"
#include "core\Serializer.h"
#include "core\FileSerializer.h"
#include "core\IProgressObserver.h"


///////////////////////////////////////////////////////////////////////////////

ResourcesManager ResourcesManager::s_theInstance;

///////////////////////////////////////////////////////////////////////////////

ResourcesManager::ResourcesManager()
: m_filesystem( new Filesystem() )
, m_progressObserverCreator( NULL )
{
   
}

///////////////////////////////////////////////////////////////////////////////

ResourcesManager::~ResourcesManager()
{
   // delete the loaders
   for ( ResourceLoadersMap::iterator it = m_loaders.begin(); it != m_loaders.end(); ++it )
   {
      delete it->second;
   }
   m_loaders.clear();

   delete m_progressObserverCreator; m_progressObserverCreator = NULL;

   delete m_filesystem; m_filesystem = NULL;

   reset();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::reset()
{
   unsigned int componentsCount = getComponentsCount();

   while( m_resources.empty() == false )
   {
      Resource* resource = m_resources.begin()->second;
      if ( resource )
      {
         // inform the resource about the components being removed
         for ( unsigned int compIdx = 0; compIdx < componentsCount; ++compIdx )
         {
            Component< ResourcesManager >* comp = getComponent( compIdx );
            resource->onComponentRemoved( *comp );
         }
      }
      delete resource;
   }
   m_resources.clear();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::setFilesystem( Filesystem* filesystem )
{
   if (filesystem == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a Filesystem instance");
   }

   // we need to delete all resources along with changing the filesystem
   reset();

   delete m_filesystem;
   m_filesystem = filesystem;
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::addResource( Resource* resource )
{
   if ( resource == NULL )
   {
      throw std::invalid_argument("NULL pointer instead a Resource instance");
   }

   if ( resource->isManaged() )
   {
      return;
   }

   ResourcesMap::iterator it = m_resources.find( resource->getFilePath() );
   if ( it == m_resources.end() )
   {
      m_resources.insert( std::make_pair( resource->getFilePath(), resource ) );
      resource->setResourcesManager( *this );
   }

   // inform the resource about the registered components
   unsigned int count = getComponentsCount();
   for ( unsigned int i = 0; i < count; ++i )
   {
      resource->onComponentAdded( *getComponent( i ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::moveResource( Resource* resource, const std::string& newPath )
{
   if ( resource == NULL )
   {
      throw std::invalid_argument("NULL pointer instead a Resource instance");
   }

   if ( resource->isManaged() )
   {
      return;
   }

   ResourcesMap::iterator it = m_resources.find( resource->getFilePath() );
   if ( it == m_resources.end() )
   {
      return;
   }

   resource->setFilePath( newPath );
   m_resources.erase( it );
   m_resources.insert( std::make_pair( newPath, resource ) );
}

///////////////////////////////////////////////////////////////////////////////

Resource* ResourcesManager::findResource( const std::string& name )
{
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

void ResourcesManager::scan( const std::string& rootDir, FilesystemScanner& scanner, bool recursive ) const
{
   // scan the contents of the filesystem
   m_filesystem->scan( rootDir, scanner, recursive );

   // scan the registered resources
   const std::size_t rootDirNameLen = rootDir.length();
   for ( ResourcesMap::const_iterator it = m_resources.begin(); it != m_resources.end(); ++it )
   {
      std::size_t rootDirPos = it->first.find( rootDir );
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
         std::size_t subDirCharPos = it->first.find_first_of( "/\\", rootDirNameLen );
         if ( subDirCharPos == std::string::npos )
         {
            scanner.onFile( it->first );
         }
      }

   }
}

///////////////////////////////////////////////////////////////////////////////

Resource& ResourcesManager::create( const std::string& name )
{
   Resource* res = findResource( name );
   if ( res == NULL )
   {
      ResourceLoader* loader = createResourceLoader( name );
      IProgressObserver* observer = createObserver();
      res = loader->load( name, *this, *observer );
      delete loader;
      delete observer;

      if ( res )
      {
         addResource( res );
      }
   }
   
   ASSERT_MSG( res != NULL, "Resource not found" );

   return *res;
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::free( Resource* resource )
{
   if ( resource == NULL )
   {
      return;
   }

   ResourcesMap::iterator it = m_resources.find( resource->getResourceName() );
   if ( it != m_resources.end() && it->second != resource )
   {
      char errMsg[512];
      sprintf_s( errMsg, "Resources discrepancy - two resources share the same name '%s'", resource->getResourceName().c_str() );
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

void ResourcesManager::save( const std::string& name, ExternalDependenciesSet& outExternalDependencies )
{
   Resource* res = findResource( name );
   if ( res != NULL )
   {
      std::string extension = Filesystem::extractExtension( name );
      std::ios_base::openmode fileAccessMode = Resource::getFileAccessMode( extension );

      File* file = m_filesystem->open( name, std::ios_base::out | fileAccessMode );
      Saver saver( new FileSerializer( file ) );
      saver.save( *res, outExternalDependencies );
   }
}

///////////////////////////////////////////////////////////////////////////////

ResourceLoader* ResourcesManager::createResourceLoader( const std::string& name ) const
{
   std::string extension = Filesystem::extractExtension( name );

   ResourceLoadersMap::const_iterator it = m_loaders.find( extension );
   if ( it != m_loaders.end() )
   {
      ResourceLoader* loader = it->second->create();
      return loader;
   }
   else
   {
      return new DefaultResourceLoader();
   }
}

///////////////////////////////////////////////////////////////////////////////

IProgressObserver* ResourcesManager::createObserver()
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
   for ( ResourcesMap::iterator it = m_resources.begin(); 
      it != m_resources.end(); ++it )
   {
      it->second->onComponentRemoved( component );
   }
}

///////////////////////////////////////////////////////////////////////////////
