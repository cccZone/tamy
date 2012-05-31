#include "core/ReflectionLoader.h"
#include "core/InStream.h"
#include "core/InArrayStream.h"
#include "core/ReflectionType.h"
#include "core/ReflectionObject.h"
#include "core/ReflectionObjectsTracker.h"
#include "core/ResourcesManager.h"
#include "core/ReflectionSerializationMacros.h"



///////////////////////////////////////////////////////////////////////////////

ReflectionLoader::ReflectionLoader( ReflectionObjectsTracker* tracker )
   : m_instancesTracker( tracker )
{
}

///////////////////////////////////////////////////////////////////////////////

ReflectionLoader::~ReflectionLoader()
{
   // remove the remaining unread objects
   for ( std::list< ReflectionObject* >::iterator it = m_loadedObjects.begin(); it != m_loadedObjects.end(); ++it )
   {
      delete *it;
   }
   m_loadedObjects.clear();
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionLoader::deserialize( InStream& inStream, std::vector< FilePath >* outDependenciesToLoad, std::vector< FilePath >* outRemappedDependencies )
{
   // 1. load external dependencies
   uint firstExternalDependencyIdx = loadExternalDependencies( inStream, outDependenciesToLoad, outRemappedDependencies );

   // 2. load internal dependencies
   bool result = loadInternalDependencies( inStream, firstExternalDependencyIdx );
   if ( !result )
   {
      // no dependencies restored - bail
      return;
   }

   // 3. load the indices of serialized objects
   uint serializedObjectsCount = 0;
   inStream >> serializedObjectsCount;
   if ( serializedObjectsCount == 0 )
   {
      // no serialized objects - something's wrong
      ASSERT_MSG( serializedObjectsCount > 0, "There are no serialized objects indices, even though we managed to load some dependencies" );
      
      // delete the deserialized dependencies
      for ( std::vector< ReflectionObject* >::iterator it = m_dependencies.begin(); it != m_dependencies.end(); ++it )
      {
         delete *it;
      }
      m_dependencies.clear();
      return;
   }

   // deserialize the objects
   for ( uint i = 0; i < serializedObjectsCount; ++i )
   {
      uint serializedObjectIdx = -1;
      inStream >> serializedObjectIdx;
      
      m_loadedObjects.push_back( m_dependencies[serializedObjectIdx] );
   }

   // copy all encountered dependencies to the list of all objects this loader's ever loaded
   uint dependenciesCount = m_dependencies.size();
   for ( uint i = 0; i < dependenciesCount; ++i )
   {
      m_allLoadedObjects.push_back( m_dependencies[i] );
   }

   // clean temporary data
   m_dependencies.clear();
}

///////////////////////////////////////////////////////////////////////////////

uint ReflectionLoader::loadExternalDependencies( InStream& stream, std::vector< FilePath >* outDependenciesToLoad, std::vector< FilePath >* outRemappedDependencies )
{
   uint remappingStartOffset = 0;

   // load the skip size first - if no array was specified for the results, we'll just skip this section of the stream
   uint skipSize = 0;
   stream >> skipSize;

   if ( outDependenciesToLoad && outRemappedDependencies )
   {
      remappingStartOffset = outRemappedDependencies->size();

      // deserialize the dependencies
      uint dependenciesCount = 0;
      stream >> dependenciesCount;
      for ( uint i = 0; i < dependenciesCount; ++i )
      {
         FilePath path;
         stream >> path;

         // append the path to the list of remapped dependencies - it's a huge continuous array where every loaded resource
         // will append its own external dependencies and its internal pointers will be remapped to point to that
         // very area in the array
         outRemappedDependencies->push_back( path );

         // as for the list of additional dependencies to load, we need to make sure 
         // the path doesn't duplicate any of the existing entries. Only then can we add it there
         bool isUnique = true;
         uint currDependenciesCount = outDependenciesToLoad->size();
         for ( uint j = 0; j < currDependenciesCount; ++j )
         {
            if ( (*outDependenciesToLoad)[j] == path )
            {
               isUnique = false;
               break;
            }
         }

         if ( isUnique )
         {
            outDependenciesToLoad->push_back( path );
         }
      }
   }
   else
   {
      // skip the dependencies
      stream.skip( skipSize );
   }

   return remappingStartOffset;
}

///////////////////////////////////////////////////////////////////////////////

bool ReflectionLoader::loadInternalDependencies( InStream& stream, uint firstExternalDependencyIdx )
{
   uint dependenciesCount = 0;
   stream >> dependenciesCount;
   if ( dependenciesCount == 0 )
   {
      return false;
   }

   for ( uint i = 0; i < dependenciesCount; ++i )
   {
      // first load the dependency id
      ReflectionObject::UniqueId objId;
      stream >> objId;

      uint skipSize = 0;
      stream >> skipSize;

      // if we are using an instances tracker, so see if such an object already
      // exists in the system, maybe there's no need  deserializing it
      ReflectionObject* dependency = NULL;
      if ( m_instancesTracker && ( dependency = m_instancesTracker->findInstance( objId ) ) != NULL )
      {
         // yep - it's there, just take the existing instance and skip that part of the stream
         stream.skip( skipSize );
      }
      else
      {
         // this is a new instance - load it
         dependency = SerializableReflectionType::load< ReflectionObject >( stream );

         // and if we're using an instances tracker, add it to it
         if ( m_instancesTracker )
         {
            m_instancesTracker->trackInstance( dependency );
         }
      }
      
      m_dependencies.push_back( dependency );
   }

   // store the remapping offset for the callback
   m_externalDependenciesOffset = firstExternalDependencyIdx;

   // now that we have the dependencies, it's time to map them in the loaded objects,
   // and remap the external dependency pointers to match the dependencies order stored in the array 'outExternalDependencies'
   for ( uint i = 0; i < dependenciesCount; ++i )
   {
      ReflectionObject* object = m_dependencies[i];
      restoreDependencies( object );
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionLoader::restoreDependencies( ReflectionObject* object ) const
{
   const SerializableReflectionType& objType = object->getVirtualRTTI();

   // create a list of classes in the inheritance hierarchy and start restoring their dependencies
   std::list< const SerializableReflectionType* > reflectionTypesList;
   objType.mapTypesHierarchy( reflectionTypesList );

   // restore the dependencies on particular subtypes
   Array< byte > tempSerializationDataBuf;
   while( !reflectionTypesList.empty() )
   {
      const SerializableReflectionType* nextType = reflectionTypesList.front();
      reflectionTypesList.pop_front();

      unsigned int membersCount = nextType->m_memberFields.size();
      for ( uint i = 0; i < membersCount; ++i )
      {
         ReflectionTypeComponent* member = nextType->m_memberFields[i];
         member->restoreDependencies( object, *this );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

ReflectionObject* ReflectionLoader::findDependency( uint dependencyIdx ) const
{
   if ( IS_DEPENDENCY_NULL( dependencyIdx ) )
   {
      // this is a NULL reference
      return NULL;
   }
   else
   {
      bool isExternal = false;
      uint extractedDependencyIdx;    
      EXTRACT_DEPENDENCY_INDEX( dependencyIdx, extractedDependencyIdx, isExternal );

      if ( isExternal )
      {
         // remap the external dependencies by the remapping offset in this pass
         uint remappedIdx = CREATE_EXTERNAL_DEPENDENCY_INDEX( extractedDependencyIdx + m_externalDependenciesOffset );
         return ( ReflectionObject* )remappedIdx;
      }
      else
      {
         return extractedDependencyIdx < m_dependencies.size() ? m_dependencies[extractedDependencyIdx] : NULL;
      }     
      return NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ExternalDependenciesMapper::ExternalDependenciesMapper( const std::vector< FilePath >& externalDependencies )
   : m_externalDependencies( externalDependencies )
{
}

///////////////////////////////////////////////////////////////////////////////

void ExternalDependenciesMapper::mapDependencies( const std::vector< ReflectionObject* >& objectsToProcess )
{
   std::list< const SerializableReflectionType* > reflectionTypesList;

   uint count = objectsToProcess.size();
   for ( uint i = 0; i < count; ++i )
   {
      ReflectionObject* object = objectsToProcess[i];
      const SerializableReflectionType& objType = object->getVirtualRTTI();

      // create a list of classes in the inheritance hierarchy and start restoring their dependencies
      reflectionTypesList.clear();
      objType.mapTypesHierarchy( reflectionTypesList );

      // restore the dependencies on particular subtypes
      Array< byte > tempSerializationDataBuf;
      while( !reflectionTypesList.empty() )
      {
         const SerializableReflectionType* nextType = reflectionTypesList.front();
         reflectionTypesList.pop_front();

         unsigned int membersCount = nextType->m_memberFields.size();
         for ( uint i = 0; i < membersCount; ++i )
         {
            ReflectionTypeComponent* member = nextType->m_memberFields[i];
            member->restoreDependencies( object, *this );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

ReflectionObject* ExternalDependenciesMapper::findDependency( uint dependencyIdx ) const
{
   if ( IS_DEPENDENCY_NULL( dependencyIdx ) )
   {
      // this is a NULL reference
      return NULL;
   }
   else
   {
      bool isExternal = false;
      uint extractedDependencyIdx;    
      EXTRACT_DEPENDENCY_INDEX( dependencyIdx, extractedDependencyIdx, isExternal );

      // external dependencies mapping pass
      if ( isExternal )
      {
         // reach to the active ResourcesManager instance and query the resource indicated by the dependency
         const FilePath& resourcePath = m_externalDependencies[ extractedDependencyIdx ];
         ResourcesManager& resMgr = ResourcesManager::getInstance();
         Resource* res = resMgr.findResource( resourcePath );
         return res;
      }
      else
      {
         // we're mapping the external dependencies only - internal dependencies already have the correct pointer defined
         // so we need to return the argument as a pointer WITHOUT altering it
         return ( ReflectionObject* )dependencyIdx;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
