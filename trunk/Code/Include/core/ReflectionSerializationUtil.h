/// @file   core/ReflectionSerializationUtil.h
/// @brief  the main serialization tool
#pragma once

#include <vector>


///////////////////////////////////////////////////////////////////////////////

class FilePath;
class ReflectionObject;
class Resource;
class IProgressObserver;

///////////////////////////////////////////////////////////////////////////////

/**
 * The main serialization tool.
 */
class ReflectionSerializationUtil
{
public:
   // -------------------------------------------------------------------------
   // Standalone objects
   // -------------------------------------------------------------------------
   /**
    * Saves a single reflection object. Use this method to serialize
    * a standalone object that DOESN'T depend on any Resource instances.
    *
    * CAUTION: an assertion will be thrown if the object depends on a Resource.
    *
    * @param object
    * @param savePath
    * @param progressObserver ( optional )
    */
   static void saveObject( const ReflectionObject* object, const FilePath& savePath, IProgressObserver* progressObserver = NULL );

   /**
    * Saves a group of reflection objects in one file. Use this method to serialize
    * standalone objects that DON'T depend on any Resource instances.
    *
    * CAUTION: an assertion will be thrown if any of the objects depends on Resource instances.
    *
    * @param objects
    * @param savePath
    * @param progressObserver ( optional )
    */
   static void saveObjects( std::vector< const ReflectionObject* >& objects, const FilePath& savePath, IProgressObserver* progressObserver = NULL );

   /**
    * A counterpart of the 'saveObject' method that saves a single object. Loads a single object
    * from the specified file.
    *
    * CAUTION: an assertion will be thrown if it turns out that the file was serialized sing 'saveObjects'
    * method or if the objects have any dependencies on the resources.
    *
    * @param loadPath
    * @param progressObserver ( optional )
    * @return  a pointer to the loaded object
    */
   static ReflectionObject* loadObject( const FilePath& loadPath, IProgressObserver* progressObserver = NULL );

   /**
    * A counterpart of the 'saveObjects' method that saves a single object. Loads all objects that were
    * serialized into the specified file. Can be also used for the files created using the 'saveObject' method.
    *
    * CAUTION: an assertion will be thrown if any of the objects has any dependencies on any resources.
    *
    * @param loadPath
    * @param outObjects 
    * @param progressObserver ( optional )
    */
   static void loadObjects( const FilePath& loadPath, std::vector< ReflectionObject* >& outObjects, IProgressObserver* progressObserver = NULL );

   // -------------------------------------------------------------------------
   // Resources
   // -------------------------------------------------------------------------
   /**
    * Saves a single resource into a dedicated file.
    *
    * @param resource
    * @param progressObserver ( optional )
    */
   static void saveResource( const Resource* resource, IProgressObserver* progressObserver = NULL );

   /**
    * Saves a group of resources into the respective dedicated files.
    *
    * @param resources
    * @param progressObserver ( optional )
    */
   static void saveResources( const std::vector< const Resource* >& resources, IProgressObserver* progressObserver = NULL );

   /**
    * Loads a single resource from the specified file.
    * All loaded resources will be added to the active ResourcesManager instance.
    *
    * @param loadPath
    * @param progressObserver ( optional )
    * @return  a pointer to the loaded resource
    */
   static Resource* loadResource( const FilePath& loadPath, IProgressObserver* progressObserver = NULL );

   /**
    * Loads and returns all the resources the file contains or references.
    * All loaded resources will be added to the active ResourcesManager instance.
    *
    * @param loadPath
    * @param outResources
    * @param progressObserver ( optional )
    */
   static void loadResources( const FilePath& loadPath, std::vector< Resource* >& outResources, IProgressObserver* progressObserver = NULL );

   // -------------------------------------------------------------------------
   // Tools
   // -------------------------------------------------------------------------

   /**
    * Collects paths to resources the specified object references ( hierarchically - this means
    * that it's gonna map the resources the immediately referenced resources reference and so on... )
    *
    * @param objectToMap
    * @param outDependenciesPaths
    */
   static void collectExternalDependencies( const ReflectionObject* objectToMap, std::vector< FilePath >& outDependenciesPaths ); 
};

///////////////////////////////////////////////////////////////////////////////
