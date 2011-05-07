#ifndef _RESOURCES_MANAGER_H
#define _RESOURCES_MANAGER_H

/// @file   core\ResourcesManager.h
/// @brief  manager of file resources used by the game

#include <map>
#include <string>
#include "core\Serializer.h"
#include "core\ComponentsManager.h"


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class Resource;

///////////////////////////////////////////////////////////////////////////////

/**
 * This manager manages the lifetime and accessibility of resources that can 
 * be loaded from files and usually contain large amount of data we don't want 
 * to scatter the precious memory with.
 *
 * Since there can only be a single instance of a resource manager ( there's no point
 * in having two instances managing resources ), this class is made into a singleton
 */
class ResourcesManager : public ComponentsManager< ResourcesManager >
{
private:
   typedef std::map<std::string, Resource* > ResourcesMap;

private:
   static ResourcesManager    s_theInstance;

   Filesystem*                m_filesystem;
   ResourcesMap               m_resources;

   friend class Resource;

public:
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
   inline Filesystem& getFilesystem();

   /**
    * Returns the currently set filesystem (const version).
    *
    * @return filesystem
    */
   inline const Filesystem& getFilesystem() const;

   /**
    * Returns the number of currently managed resources.
    */
   inline unsigned int getResourcesCount() const;

   /**
    * Registers a new resource instance with the resources manager.
    *
    * @param resource   new resource instance to register
    */
   void addResource( Resource* resource );

   /**
    * Moves an existing resource to a different path.
    *
    * @param resource
    * @param newPath
    */
   void moveResource( Resource* resource, const std::string& newPath );

   /**
    * Creates a resource based on a file with the specified name and registers it
    * with the manager, if it hasn't been registered before.
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

   /**
    * Returns the singleton instance of the resources manager.
    */
   static inline ResourcesManager& getInstance() { return s_theInstance; }

protected:
   // -------------------------------------------------------------------------
   // ComponentsManager implementation
   // -------------------------------------------------------------------------
   void onComponentAdded( Component< ResourcesManager >& component );
   void onComponentRemoved( Component< ResourcesManager >& component );

   /**
    * Releases the resource from the memory, leaving it untouched in the filesystem.
    *
    * @param resource
    */
   void free( Resource* resource );

private:
   /**
    * Constructor.
    */
   ResourcesManager();
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ResourcesManager.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCES_MANAGER_H
