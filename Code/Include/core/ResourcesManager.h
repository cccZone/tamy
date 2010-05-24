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
 */
class ResourcesManager : public ComponentsManager< ResourcesManager >
{
private:
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

protected:
   // -------------------------------------------------------------------------
   // ComponentsManager implementation
   // -------------------------------------------------------------------------
   void onComponentAdded( Component< ResourcesManager >& component );
   void onComponentRemoved( Component< ResourcesManager >& component );
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ResourcesManager.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCES_MANAGER_H
