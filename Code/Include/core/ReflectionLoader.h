/// @file   core/ReflectionLoader.h
/// @brief  serialization tool for loading reflection objects
#ifndef _REFLECTION_LOADER_H
#define _REFLECTION_LOADER_H

#include <vector>
#include <list>
#include "core/types.h"
#include "core/FilePath.h"
#include "core/ReflectionDependenciesCallback.h"


///////////////////////////////////////////////////////////////////////////////

class InStream;
class ReflectionObject;
class ReflectionObjectsTracker;

///////////////////////////////////////////////////////////////////////////////

/**
 * Serialization tool for loading archives of serialized objects.
 */
class ReflectionLoader : public ReflectionDependencyLinkerCallback
{
public:
   // list of ALL objects loaded from the archive
   std::list< ReflectionObject* >            m_allLoadedObjects;

   // list of the main objects loaded from the archive ( those which the user passed as an argument to ReflectionSaver.save method )
   std::list< ReflectionObject* >            m_loadedObjects;

private:
   ReflectionObjectsTracker*                 m_instancesTracker;

   // temporary data used in the callback methods
   // dependencies buffer used when initial dependencies are being loaded
   std::vector< ReflectionObject* >          m_dependencies;

   // remapping offset for the external dependencies
   uint                                      m_externalDependenciesOffset;

public:
   /**
    * Constructor.
    *
    * @param tracker    instances tracker
    */
   ReflectionLoader( ReflectionObjectsTracker* tracker = NULL );
   ~ReflectionLoader();

   /**
    * Deserializes new data from the stream.
    *
    * @param inStream
    * @param outDependenciesToLoad           if specified, it will tell the caller what other files should be loaded
    *                                        to make this one complete
     * @param outRemappedDependencies         NEEDS TO BE specified ALONG with the outDependenciesToLoad.
     *                                        This object's external dependencies will be APPENDED to this array, 
    *                                        and its internal dependency indices will be updated to map to those names, so that
    *                                        later on the object and the final dependencies list can be passed to the ExternalDependenciesLinker
   *                                        which will finish the loading process.
    */
   void deserialize( InStream& inStream, std::vector< FilePath >* outDependenciesToLoad = NULL, std::vector< FilePath >* outRemappedDependencies = NULL );

   /**
    * Retrieves the next loaded object.
    *
    * CAUTION: be sure to use the ExternalDependenciesLinker if you expect the loaded object to have any external resources.
    *
    * @param T                               expected object type
    *
    * @return                                deserialized object
    */
   template< typename T >
   T* getNextObject();

   // -------------------------------------------------------------------------
   // ReflectionDependenciesCallback implementation
   // -------------------------------------------------------------------------
   ReflectionObject* findDependency( uint dependencyIdx ) const;

private:
   uint loadExternalDependencies( InStream& stream, std::vector< FilePath >* outDependenciesToLoad, std::vector< FilePath >* outRemappedDependencies );
   bool loadInternalDependencies( InStream& stream, uint firstExternalDependencyIdx );
   
   /**
    * Restores the dependencies on the objects the specified object references.
    *
    * @param object                          restored object
    */
   void restoreDependencies( ReflectionObject* object ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A tool that links the external dependencies between the deserialized objects.
 */
class ExternalDependenciesLinker : public ReflectionDependencyLinkerCallback
{
private:
   const std::vector< FilePath >&         m_externalDependencies;

public:
   /**
    * Constructor.
    *
    * @param externalDependencies
    */
   ExternalDependenciesLinker( const std::vector< FilePath >& externalDependencies );

   /**
    * Fills the pointers to Resource objects that weren't loaded during the standard loading process.
    *
    * In order to do that, it queries the active instance of ResourcesManager for the resources from the 'externalDependencies' array
    * whenever it finds that one of the objects from the array of 'objectsToProcess' references one.
    *
    * @param objectsToProcess
    */
   void linkDependencies( const std::vector< ReflectionObject* >& objectsToProcess );

   // -------------------------------------------------------------------------
   // ReflectionDependenciesCallback implementation
   // -------------------------------------------------------------------------
   ReflectionObject* findDependency( uint dependencyIdx ) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core/ReflectionLoader.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_LOADER_H
