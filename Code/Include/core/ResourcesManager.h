#ifndef _RESOURCES_MANAGER_H
#define _RESOURCES_MANAGER_H

/// @file   core\ResourcesManager.h
/// @brief  manager of file resources used by the game

#include <map>
#include <string>
#include "core\SingletonsManager.h"


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

   typedef std::vector<Resource*> ResourcesVec;
   typedef std::map<std::string, ResourcesVec > ResourcesMap;
   ResourcesMap m_resources;
   unsigned int m_resourcesCount;

public:
   /**
    * Constructor.
    */
   ResourcesManager();
   ~ResourcesManager();

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
    * @param name       name of the resource file (should exist in the
    *                   currently set filesystem)
    * @param RES_TYPE   resource class type
    */
   template<typename RES_TYPE>
   void addResource(RES_TYPE* resource, const std::string& name);

   /**
    * Creates a resource based on a file with the specified name.
    *
    * @param name       name of the resource file (should exist in the
    *                   currently set filesystem)
    * @param RES_TYPE   resource class type
    */
   template<typename RES_TYPE>
   RES_TYPE& create(const std::string& name);

private:
   template<typename RES_TYPE>
   RES_TYPE* findResource(ResourcesVec& resources);
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ResourcesManager.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCES_MANAGER_H
