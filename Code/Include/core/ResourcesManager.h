#ifndef _RESOURCES_MANAGER_H
#define _RESOURCES_MANAGER_H

/// @file   core\ResourcesManager.h
/// @brief  manager of file resources used by the game

#include <map>
#include <string>
#include "core\Serializer.h"
#include "core\ComponentsManager.h"
#include "core\Filesystem.h"


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class Resource;
class ResourceLoader;
class IProgressObserver;
class FilesystemScanner;

///////////////////////////////////////////////////////////////////////////////

/**
 * This manager manages the lifetime and accessibility of resources that can 
 * be loaded from files and usually contain large amount of data we don't want 
 * to scatter the precious memory with.
 *
 * Since there can only be a single instance of a resource manager ( there's no point
 * in having two instances managing resources ), this class is made into a singleton
 */
class ResourcesManager : public ComponentsManager< ResourcesManager >, public FilesystemListener
{
private:
   template< typename T >
   class InstanceCreator
   {
   public:
      virtual ~InstanceCreator() {}
      
      /**
       * Instantiates a class.
       */
      virtual T* create() const = 0;
   };

   typedef InstanceCreator< ResourceLoader >       ResourceLoaderCreator;
   template< typename T >
   class TResourceLoaderCreator : public ResourceLoaderCreator
   {
   public:
      ResourceLoader* create() const
      {
         return new T();
      }
   };

   typedef InstanceCreator< IProgressObserver >    ProgressObserverCreator;
   template< typename T >
   class TProgressObserverCreator : public ProgressObserverCreator
   {
   public:
      IProgressObserver* create() const
      {
         return new T();
      }
   };

private:
   typedef std::map< std::string, Resource* >               ResourcesMap;
   typedef std::map< std::string, ResourceLoaderCreator* >  ResourceLoadersMap;

private:
   static ResourcesManager    s_theInstance;

   Filesystem*                m_filesystem;
   ResourcesMap               m_resources;
   ResourceLoadersMap         m_loaders;
   ProgressObserverCreator*   m_progressObserverCreator;

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
    * Creates a resource based on a file with the specified name and registers it
    * with the manager, if it hasn't been registered before.
    *
    * @param name       name of the resource file (should exist in the
    *                   currently set filesystem)
    */
   Resource& create( const std::string& name );

   /**
    * Registers a new resource instance with the resources manager.
    *
    * @param resource   new resource instance to register
    * @return           'true' if the resource was added, 'false' otherwise ( in the latter case the passed resource
    *                   will automatically get deleted
    */
   bool addResource( Resource* resource );

   /**
    * Looks up a resource with the given name of the specified type.
    *
    * @param name       name of the resource
    * @return           pointer to the resource or NULL if the manager does not have one.
    */
   template< typename RESOURCE_TYPE >
   RESOURCE_TYPE* findResource( const std::string& name );

   /**
    * Looks for a resource with the specified name ( utility method )
    *
    * @param name
    */
   Resource* findResource( const std::string& name );

   /**
    * Moves an existing resource to a different path.
    *
    * @param resource
    * @param newPath
    */
   void moveResource( Resource* resource, const std::string& newPath );

   /**
    * Saves a resource associated with the specified file onto the filesystem.
    *
    * @param name       name of the resource file (should exist in the
    *                   currently set filesystem)
    * @param outExternalDependencies   other resources used by this resource
    */
   void save( const std::string& name, ExternalDependenciesSet& outExternalDependencies = ExternalDependenciesSet() );

   /**
    * The method scans the resource manager memory in search for loaded resources, 
    * starting from the specified root directory, and informs via the FilesystemScanner 
    * interface about its finding.
    *
    * @param rootDir    directory from which the scanning should begin
    * @param scanner
    * @param recursive  use recursive search through the directories tree
    */
   void scan( const std::string& rootDir, FilesystemScanner& scanner, bool recursive = true ) const;

   /**
    * Registers a loader of the specified type.
    */
   template< typename T >
   void addLoader( const std::string& extension );

   /**
    * Registers a class that will be used as a progress observer
    */
   template< typename T >
   void setProgressObserver();

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

   // -------------------------------------------------------------------------
   // FilesystemListener implementation
   // -------------------------------------------------------------------------
   void onDirChanged( const std::string& dir );

   /**
    * Called by the resource when it gets deleted in order to remove
    * its record from the manager.
    *
    * @param resource
    */
   void free( Resource* resource );

private:
   /**
    * Constructor.
    */
   ResourcesManager();

   /**
    * Creates a resource loader for the specified extension
    */
   ResourceLoader* createResourceLoader( const std::string& extension ) const;

   /**
    * Creates a progress observer.
    */
   IProgressObserver* createObserver() const;

   /**
    * Helper method for loading a resource from the filesystem.
    *
    * @param loader
    */
   Resource* loadResource( ResourceLoader& loader );
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ResourcesManager.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCES_MANAGER_H
