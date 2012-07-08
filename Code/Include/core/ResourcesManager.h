#ifndef _RESOURCES_MANAGER_H
#define _RESOURCES_MANAGER_H

/// @file   core\ResourcesManager.h
/// @brief  manager of file resources used by the game

#include <map>
#include <string>
#include "core\ComponentsManager.h"
#include "core\Filesystem.h"
#include "core\ResourceImporter.h"


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class Resource;
class IProgressObserver;
class FilesystemScanner;
class FilePath;
class ReflectionSerializationUtil;

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

   // -------------------------------------------------------------------------

   class ResourceImporterCreator
   {
   public:
      virtual ~ResourceImporterCreator() {}
      
      /**
       * Instantiates an importer class.
       */
      virtual ResourceImporter* create( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer ) const = 0;
   };

   template< typename T >
   class TResourceImporterCreator : public ResourceImporterCreator
   {
   public:
      ResourceImporter* create( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer ) const
      {
         return new T( path, rm, observer );
      }
   };

   // -------------------------------------------------------------------------

   class ProgressObserverCreator
   {
   public:
      virtual ~ProgressObserverCreator() {}
      
      /**
       * Instantiates a progress observer class.
       */
      virtual IProgressObserver* create() const = 0;
   };

   template< typename T >
   class TProgressObserverCreator : public ProgressObserverCreator
   {
   public:
      IProgressObserver* create() const
      {
         return new T();
      }
   };

   // -------------------------------------------------------------------------


private:
   typedef std::map< FilePath, Resource* >                     ResourcesMap;

   typedef std::vector< ResourceImporterCreator* >             ImportersArr;
   typedef std::map< std::string, ImportersArr* >   ResourceImportersMap;

private:
   static ResourcesManager    s_theInstance;

   Filesystem*                m_filesystem;
   ResourcesMap               m_resources;
   ResourceImportersMap       m_importers;
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
   void setFilesystem( Filesystem* filesystem );

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
    * @param loadOnly   if set to 'true', the manager won't attempt to create a new resource
    *                   if it doesn't already exist in the filesystem
    */
   template< typename RESOURCE_TYPE >
   RESOURCE_TYPE* create( const FilePath& name, bool loadOnly = false );

   /**
    * Creates a resource based on a file with the specified name and registers it
    * with the manager, if it hasn't been registered before.
    *
    * @param name       name of the resource file (should exist in the
    *                   currently set filesystem)
    * @param loadOnly   if set to 'true', the manager won't attempt to create a new resource
    *                   if it doesn't already exist in the filesystem
    */
   Resource* create( const FilePath& name, bool loadOnly = false );

   /**
    * Removes a resource corresponding to the specified filepath.
    * It will remove both the memory and the filesystem representation of the resource.
    *
    * If it's not a resource path that has been specified, or it's a resource that
    * hasn't been loaded into the system memory, the item will be deleted
    * either way.
    *
    * @param path       path to a resource we want to remove
    */
   void remove( const FilePath& path );

   /**
    * Registers a new resource instance with the resources manager and notifies it about this fact
    * by calling the onResourceLoaded method and by registering all active components with it.
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
   RESOURCE_TYPE* findResource( const FilePath& name );

   /**
    * Looks for a resource with the specified name ( utility method )
    *
    * @param name
    */
   Resource* findResource( const FilePath& name );

   /**
    * Moves an existing resource to a different path.
    *
    * @param resource
    * @param newPath
    */
   void moveResource( Resource* resource, const FilePath& newPath );

   /**
    * Saves a resource associated with the specified file onto the filesystem.
    *
    * @param name       name of the resource file (should exist in the
    *                   currently set filesystem)
    * @param outExternalDependencies   other resources used by this resource
    */
   void save( const FilePath& name );

   /**
    * The method scans the resource manager memory in search for loaded resources, 
    * starting from the specified root directory, and informs via the FilesystemScanner 
    * interface about its finding.
    *
    * @param rootDir    directory from which the scanning should begin
    * @param scanner
    * @param recursive  use recursive search through the directories tree
    */
   void scan( const FilePath& rootDir, FilesystemScanner& scanner, bool recursive = true ) const;

   // -------------------------------------------------------------------------
   // Importers management
   // -------------------------------------------------------------------------
   /**
    * Registers a loader of the specified type.
    */
   template< typename Importer, typename ResourceType >
   void addImporter( const std::string& extension );

   /**
    * Creates an importer that can import the specified resource from the specified
    * file.
    *
    * @param ResourceType
    * @param path          file path
    */
   template< typename ResourceType >
   TResourceImporter< ResourceType >* createImporter( const FilePath& path );

   // -------------------------------------------------------------------------
   // Instance management
   // -------------------------------------------------------------------------
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
   void onDirAdded( const FilePath& dir );
   void onDirRemoved( const FilePath& dir );
   void onFileEdited( const FilePath& path );
   void onFileRemoved( const FilePath& path );

   /**
    * Called by the resource when it gets deleted in order to remove
    * its record from the manager.
    *
    * @param resource
    */
   void free( Resource* resource );

   // -------------------------------------------------------------------------
   // Serialization helpers
   // -------------------------------------------------------------------------

   friend class ReflectionSerializationUtil;

   /**
    * Registers a new resource with the resources manager.
    *
    * @param resource
    */
   bool registerNewResource( Resource* resource );

private:
   /**
    * Constructor.
    */
   ResourcesManager();

   /**
    * Creates a progress observer.
    */
   IProgressObserver* createObserver() const;

   /**
    * Helper method for loading a resource from the filesystem.
    *
    * @param filePath
    */
   Resource* loadResource( const FilePath& filePath );
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ResourcesManager.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCES_MANAGER_H
