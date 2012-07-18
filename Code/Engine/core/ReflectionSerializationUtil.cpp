#include "core/ReflectionSerializationUtil.h"
#include "core/FilePath.h"
#include "core/InFileStream.h"
#include "core/OutFileStream.h"
#include "core/ReflectionLoader.h"
#include "core/ReflectionSaver.h"
#include "core/Resource.h"
#include "core/ReflectionObject.h"
#include "core/IProgressObserver.h"
#include "core/ResourceDependenciesMapper.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

void ReflectionSerializationUtil::saveObject( const ReflectionObject* object, const FilePath& savePath, IProgressObserver* progressObserver )
{
   std::vector< const ReflectionObject* > objects;
   objects.push_back( object );
   saveObjects( objects, savePath, progressObserver );
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionSerializationUtil::saveObjects( std::vector< const ReflectionObject* >& objects, const FilePath& savePath, IProgressObserver* progressObserver )
{
   // open a file to which the objects will be saved and create a saver
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   Filesystem& filesystem = resMgr.getFilesystem();

   File* file = filesystem.open( savePath, std::ios_base::out | std::ios_base::binary );
   OutFileStream stream( file );
   ReflectionSaver saver( stream );

   // notify about the serialization progress
   std::vector< FilePath > externalDependencies;
   uint objectsCount = objects.size();
   if ( progressObserver ) 
   {
      progressObserver->initialize( "Saving objects", objectsCount );
   }

   // serialize the objects
   for ( uint i = 0; i < objectsCount; ++i )
   {
      saver.save( objects[i] );

      // safety measures - check if there were any external dependencies in the saved object
      saver.collectExternalDependencies( externalDependencies );
      ASSERT_MSG( externalDependencies.empty(), "The serialized object contains external dependencies and therefore will be incomplete after deserialization" );

      if ( progressObserver )
      {
         progressObserver->advance();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

ReflectionObject* ReflectionSerializationUtil::loadObject( const FilePath& loadPath, IProgressObserver* progressObserver )
{
   std::vector< ReflectionObject* > loadedObjects;

   loadObjects( loadPath, loadedObjects, progressObserver );

   return loadedObjects.empty() ? NULL : loadedObjects[0];
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionSerializationUtil::loadObjects( const FilePath& loadPath, std::vector< ReflectionObject* >& outObjects, IProgressObserver* progressObserver )
{
   // open a file to which the objects will be saved and create a saver
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   Filesystem& filesystem = resMgr.getFilesystem();

   File* file = filesystem.open( loadPath, std::ios_base::in | std::ios_base::binary );
   InFileStream stream( file );

   std::vector< FilePath > resourcesToLoad;
   std::vector< FilePath > resourcesMap;
   ReflectionLoader loader;
   loader.deserialize( stream, &resourcesToLoad, &resourcesMap );

   ASSERT_MSG( resourcesToLoad.empty(), "The archive contains references to external Resources, which won't be loaded by this method. Use 'loadResources' instead." );
   if ( !resourcesToLoad.empty() )
   {
      return;
   }

   // notify about the serialization progress
   uint objectsCount = loader.m_loadedObjects.size();
   std::vector< FilePath > externalDependencies;
   if ( progressObserver ) 
   {
      progressObserver->initialize( "Saving objects", objectsCount );
   }

   // serialize the objects
   for ( uint i = 0; i < objectsCount; ++i )
   {
      ReflectionObject* loadedObject = loader.getNextObject< ReflectionObject >();
      outObjects.push_back( loadedObject );
      if ( progressObserver )
      {
         progressObserver->advance();
      }
   }

   // last but not least, make sure that all loaded objects are informed that they were loaded
   for ( std::list< ReflectionObject* >::const_iterator it = loader.m_allLoadedObjects.begin(); it != loader.m_allLoadedObjects.end(); ++it )
   {
      ReflectionObject* object = *it;
      if ( object )
      {
         object->onObjectLoaded();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionSerializationUtil::saveResource( const Resource* resource, IProgressObserver* progressObserver )
{
   std::vector< const Resource* > resourcesToSave;
   resourcesToSave.push_back( resource );
   saveResources( resourcesToSave, progressObserver );
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionSerializationUtil::saveResources( const std::vector< const Resource* >& resources, IProgressObserver* progressObserver )
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   Filesystem& filesystem = resMgr.getFilesystem();

   // initialize the progress observer
   std::vector< FilePath > resourcesToSave;
   uint resourcesCount = resources.size();
   for ( uint i = 0; i < resourcesCount; ++i )
   {
      resourcesToSave.push_back( resources[i]->getFilePath() );
   }

   // notify about the serialization progress
   if ( progressObserver ) 
   {
      progressObserver->initialize( "Saving resources", resourcesCount );
   }


   // Go through all resources in the list and save them. New resources
   // will be added to the list as we keep mapping them.
   uint savedResourceIdx = 0;
   while( savedResourceIdx < resourcesToSave.size() )
   {
      const FilePath& savedResourcePath = resourcesToSave[savedResourceIdx];

      // locate the resource we want saved
      Resource* res = resMgr.findResource( savedResourcePath );
      ++savedResourceIdx;
      if ( res == NULL )
      {
         if ( progressObserver ) 
         {
            progressObserver->advance();
         }

         continue;
      }

      // open the file for saving
      std::string extension = savedResourcePath.extractExtension();
      std::ios_base::openmode accessMode = Resource::getFileAccessMode( extension );
      File* file = filesystem.open( savedResourcePath, std::ios_base::out | accessMode );
      if ( !file )
      {
         if ( progressObserver ) 
         {
            progressObserver->advance();
         }

         continue;
      }

      // save the resource
      OutFileStream outStream( file );
      ReflectionSaver saver( outStream );
      saver.save( res );

      // add the mapped resources to the save list, providing they are unique
      saver.collectExternalDependencies( resourcesToSave );

      // advance the observer ONLY when the resource was successfully saved
      if ( progressObserver ) 
      {
         progressObserver->advance();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

Resource* ReflectionSerializationUtil::loadResource( const FilePath& loadPath, IProgressObserver* progressObserver )
{
   std::vector< Resource* > loadedResources;
   loadResources( loadPath, loadedResources, progressObserver );

   return loadedResources.empty() ? NULL : loadedResources[0];
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionSerializationUtil::loadResources( const FilePath& loadPath, std::vector< Resource* >& outResources, IProgressObserver* progressObserver )
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   Filesystem& filesystem = resMgr.getFilesystem();


   std::vector< FilePath > resourcesToLoad;
   std::vector< FilePath > resourcesMap;
   resourcesToLoad.push_back( loadPath );

   // notify about the serialization progress
   if ( progressObserver )
   {
      progressObserver->initialize( "Loading resource", outResources.size() );
   }


   // Go through all resources in the list and load them. New resources
   // will be added to the list as we keep mapping them.
   std::vector< ReflectionObject* > loadedResources;
   std::vector< ReflectionObject* > allLoadedObjects;
   uint loadedResourceIdx = 0;
   while( loadedResourceIdx < resourcesToLoad.size() )
   {
      const FilePath& loadedResourcePath = resourcesToLoad[loadedResourceIdx];
      ++loadedResourceIdx;

      // first check if such a resource is not yet loaded ( perhaps it's already there, and we can save time )
      Resource* res = resMgr.findResource( loadedResourcePath );
      if ( res == NULL )
      {
         // open the file for loading
         std::string extension = loadedResourcePath.extractExtension();
         std::ios_base::openmode accessMode = Resource::getFileAccessMode( extension );
         File* file = filesystem.open( loadedResourcePath, std::ios_base::in | accessMode );

         if ( file )
         {
            InFileStream inStream( file );
            ReflectionLoader loader;
            loader.deserialize( inStream, &resourcesToLoad, &resourcesMap );
            res = loader.getNextObject< Resource >();

            allLoadedObjects.insert( allLoadedObjects.end(), loader.m_allLoadedObjects.begin(), loader.m_allLoadedObjects.end() );
         }
      }

      if ( res )
      {
         // memorize the loaded resource
         loadedResources.push_back( res );
      }
      else
      {
         // if any of the dependencies were not loaded, break the process
         break;
      }

      // advance the observer ONLY when the resource was successfully loaded
      if ( progressObserver )
      {
         progressObserver->advance();
      }
   }

   // Only if there are as many loaded resources as there were mapped resource names can we consider
   // loading to be successful.
   if ( loadedResources.size() == resourcesToLoad.size() )
   {
      // But before we do, let's register the resources with the manager,
      // `cause the mapper references the manager to query for those resources.
      // We're gonna need to add them anyway, so this is the perfect spot.
      uint count = loadedResources.size();
      for ( uint i = 0; i < count; ++i )
      {
         Resource* res = static_cast< Resource* >( loadedResources[i] );
         if ( !res->isManaged() )
         {
            // register the managed resources
            res->setFilePath( resourcesToLoad[i] );
            resMgr.registerNewResource( res );
         }

         // and since we're already iterating over the resources and casting them - put the resources in the output array
         outResources.push_back( res );
      }

      // it was successful - map inter-resource dependencies on all loaded objects
      ExternalDependenciesLinker linker( resourcesMap );
      linker.linkDependencies( allLoadedObjects );
   }
   else
   {
      // loading failed - delete what we created so far in the process
      uint count = loadedResources.size();
      for ( uint i = 0; i < count; ++i )
      {
         Resource* res = static_cast< Resource* >( loadedResources[i] );
         if ( !res->isManaged() )
         {
            // delete only the unmanaged resources ( we could have encountered resources that are already registered with the resources manager - DO NOT touch those )
            delete res;
         }
      }

      // and exit - no further steps should be taken at this point
      return;
   }

   // make sure that all loaded objects are informed that they were loaded
   uint allLoadedObjectsCount = allLoadedObjects.size();
   for ( uint i = 0; i < allLoadedObjectsCount; ++i )
   {
      allLoadedObjects[i]->onObjectLoaded();
   }

   // At this point all dependencies between the objects have been restored
   // and the resources have been registered with the resources manager.
   // Since it's the resources that manage the loaded objects, perform one last step -
   // inform them that everything has been successfully loaded so that they can kick off any final
   // post-load group activities
   uint allLoadedResources = loadedResources.size();
   for ( uint i = 0; i < allLoadedResources; ++i )
   {
      Resource* res = static_cast< Resource* >( loadedResources[i] );
      res->finalizeResourceLoading();
   }
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionSerializationUtil::collectExternalDependencies( const ReflectionObject* objectToMap, std::vector< FilePath >& outDependenciesPaths )
{
   // it was successful - map inter-resource dependencies on all loaded objects
   ResourceDepenenciesMapper mapper( outDependenciesPaths );
   mapper.mapDependencies( objectToMap );
}

///////////////////////////////////////////////////////////////////////////////
