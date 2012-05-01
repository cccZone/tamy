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

template< typename Importer, typename ResourceType >
void ResourcesManager::addImporter( const std::string& extension )
{
   ResourceImportersMap::iterator it = m_importers.find( extension );
   if ( it == m_importers.end() )
   {
      it = m_importers.insert( std::make_pair( extension, new ImportersArr() ) ).first;
   }

   it->second->push_back( new TResourceImporterCreator< Importer >() );
}

///////////////////////////////////////////////////////////////////////////////

template< typename ResourceType >
TResourceImporter< ResourceType >* ResourcesManager::createImporter( const FilePath& name )
{
   std::string extension = name.extractExtension();

   ResourceImportersMap::const_iterator itArr = m_importers.find( extension );

   ResourceImporter* importer = NULL;
   if ( itArr != m_importers.end() )
   {
      // go through all importers and check which one can import the desired resource type
      ImportersArr* importersArr = itArr->second;
      for ( ImportersArr::const_iterator it = importersArr->begin(); it != importersArr->end(); ++it )
      {
         IProgressObserver* observer = createObserver();
         ResourceImporter* tmpImporter = (*it)->create( name, *this, observer );   

         if ( tmpImporter && tmpImporter->canImport( ResourceType::getStaticRTTI() ) )
         {
            // found it
            importer = tmpImporter;
            break;
         }
         else
         {
            // that's not it
            delete tmpImporter;
         }
      }

   }

   return static_cast< TResourceImporter< ResourceType >* >( importer );
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
