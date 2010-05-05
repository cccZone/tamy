#ifndef _RESOURCES_MANAGER_H
#define _RESOURCES_MANAGER_H

/// @file   core\ResourcesManager.h
/// @brief  manager of file resources used by the game

#include <map>
#include <string>
#include "core\SingletonsManager.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class Resource;

///////////////////////////////////////////////////////////////////////////////

/**
 * This manager manages the lifetime and accessibility of resources that can 
 * be loaded from files and usually contain large amount of data we don't want 
 * to scatter the precious memory with.
 */
class ResourcesManager
{
private:
   SingletonsManager m_initializers;

   Filesystem* m_filesystem;

   typedef std::map<std::string, Resource* > ResourcesMap;
   ResourcesMap m_resources;

public:
   /**
    * Constructor.
    */
   ResourcesManager();
   ~ResourcesManager();

   /**
    * Reset the manager by removing all registered resources.
    */
   void reset();

   /**
    * Sets a new filesystem the manager should use to load the resources from.
    *
    * @param filesystem
    */
   void setFilesystem(Filesystem* filesystem);

   /**
    * Returns the currently set filesystem.
    *
    * @return filesystem
    */
   inline const Filesystem& getFilesystem() const;

   /**
    * Returns the number of currently managed resources.
    */
   inline unsigned int getResourcesCount() const;

   /**
    * Adds an initialization component required by some resources.
    *
    * @param instance      initialization component instance
    * @return              this resources manager instance, allowing
    *                      to chain subsequent calls to this method
    */
   template<typename INIT_COMP>
   ResourcesManager& associate(INIT_COMP& instance);

   /**
    * Returns a storage with the initialization components.
    */
   inline SingletonsManager& getInitializers();

   /**
    * Returns a storage with the initialization components (const version).
    */
   inline const SingletonsManager& getInitializers() const;

   /**
    * Registers a new resource instance with the resources manager.
    *
    * @param resource   new resource instance to register
    */
   void addResource( Resource* resource );

   /**
    * Creates a resource based on a file with the specified name.
    *
    * @param name       name of the resource file (should exist in the
    *                   currently set filesystem)
    */
   Resource& create(const std::string& name);

   /**
    * Saves a resource associated with the specified file onto the filesystem.
    *
    * @param name       name of the resource file (should exist in the
    *                   currently set filesystem)
    * @param outExternalDependencies   other resources used by this resource
    */
   void save( const std::string& name, ExternalDependenciesSet& outExternalDependencies = ExternalDependenciesSet() );

   /**
    * Looks up a resource with the given name.
    *
    * @param name       name of the resource
    * @return           pointer to the resource or NULL if the manager does not have one.
    */
   Resource* findResource( const std::string& name );
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ResourcesManager.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCES_MANAGER_H
