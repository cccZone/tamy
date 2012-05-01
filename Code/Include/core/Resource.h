/// @file   core\Resource.h
/// @brief  interface marking all resources
#ifndef _RESOURCE_H
#define _RESOURCE_H

#include "core\ReflectionObject.h"
#include "core\Component.h"
#include "core\ResourceHandle.h"
#include "core\FilePath.h"


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;
class ReflectionType;

///////////////////////////////////////////////////////////////////////////////

/**
 * This interface marks all resources that can be loaded using 
 * the resources manager.
 */
class Resource : public ReflectionObject
{
   DECLARE_CLASS()

public:
   enum AccessMode
   {
      AM_BINARY,
      AM_TEXT
   };

   friend class ResourcesManager;

private:
   FilePath                         m_filePath;
   ResourcesManager*                m_host;

   std::vector< ResourceObject* >   m_managedObjects;
   std::vector< int >               m_freeIds;

public:
   /**
    * Constructor.
    *
    * @param filePath   path to the file where we want to save the resource.
    */
   Resource( const FilePath& filePath = FilePath() );
   virtual ~Resource();

   /**
    * Returns the path to the file holding this resource.
    */
   inline const FilePath& getFilePath() const { return m_filePath; }

   /**
    * Sets a new path to the resource.
    *
    * @param path
    * @param host    host manager performing the operation
    */
   void setFilePath( const FilePath& path, ResourcesManager* host = NULL );

   /**
    * A helper method allowing to saves the resource to an associated file.
    */
   void saveResource();

   /**
    * Tells whether the resource is managed by a resources manager.
    */
   inline bool isManaged() const { return m_host != NULL; }

   /**
    * Returns an extension of this resource instance.
    */
   virtual const char* getVirtualExtension() { return ""; }


   // -------------------------------------------------------------------------
   // Resource types management
   // -------------------------------------------------------------------------

   /**
    * Tells whether the specified extension can be mapped to a specific resource.
    *
    * @param extension     resource file extension
    */
   static bool isResource( const std::string& extension );

   /**
    * A method telling how each resource file should be opened
    * (using binary or text access mode).
    *
    * @param extension     resource file extension
    */
   static std::ios_base::openmode getFileAccessMode( const std::string& extension );

   /**
    * Searches for a resource type matching the specified extension.
    *
    * @param extension
    */
   static const SerializableReflectionType* findResourceClass( const std::string& extension );

   /**
    * Registers a new resource type.
    *
    * @param extension     resource file extension
    * @param accessMode    resource file access mode
    * @param typeName      resource type name
    */
   static void registerResource( const std::string& extension, AccessMode accessMode, const std::string& typeName );

   // -------------------------------------------------------------------------
   // Managed objects
   // -------------------------------------------------------------------------
   /**
    * Returns an instance of the managed object using its ID.
    */
   ResourceObject& getObject( int objectId );

protected:
   /**
    * Informs the resource that from now on it's managed by a resources manager.
    */
   void setResourcesManager( ResourcesManager& mgr );

   /**
    * Informs the resource that it's been removed from the resources manager.
    */
   void resetResourcesManager();

   // -------------------------------------------------------------------------
   // Notifications
   // -------------------------------------------------------------------------
   /**
    * Called when a component is added to the manager managing this resource.
    *
    * @param component
    */
   virtual void onComponentAdded( Component< ResourcesManager >& component ) {}

   /**
    * Called when a component is removed from the manager managing this resource.
    *
    * @param component
    */
   virtual void onComponentRemoved( Component< ResourcesManager >& component ) {}

   /**
    * The method is called by the resources manager once
    * before the resource gets saved to give it a chance to perform pre-save state caching etc.
    */
    virtual void onResourceSave( ResourcesManager& mgr ) {}

   /**
    * The method is called by the resources manager once
    * the resource has successfully been registered with the manager.
    */
    virtual void onResourceLoaded( ResourcesManager& mgr ) {}

    // -------------------------------------------------------------------------
    // Managed objects
    // -------------------------------------------------------------------------
    /**
    * Adds a new object under the resource control.
    *
    * @param object
    */
   void addObject( ResourceObject* object );

   /**
    * Removes an object from the resource's control.
    *
    * @param handle
    */
   void removeObject( int objectId );
};

///////////////////////////////////////////////////////////////////////////////

// Resource declaration
#define DECLARE_RESOURCE()                                                    \
   DECLARE_CLASS()                                                            \
   public:                                                                    \
      static const char* getExtension();                                      \
      virtual const char* getVirtualExtension();                              \
   private:

///////////////////////////////////////////////////////////////////////////////

// Declares a specific resourceBEGIN_RESOURCE_NO_PARENT
#define BEGIN_RESOURCE_NO_PARENT( ClassName, Extension, AccessMode )          \
   class RegisterResource##Extension                                          \
   {                                                                          \
   public:                                                                    \
      RegisterResource##Extension()                                           \
      {                                                                       \
         ReflectionTypeID< ClassName > type;                                  \
         Resource::registerResource( #Extension, Resource::AccessMode, type.m_name ); \
      }                                                                       \
   };                                                                         \
   RegisterResource##Extension resourceTypeRegistryFor_##Extension;           \
   const char* ClassName::getExtension() { return #Extension; }               \
   const char* ClassName::getVirtualExtension() { return ClassName::getExtension(); } \
   BEGIN_OBJECT( ClassName );

///////////////////////////////////////////////////////////////////////////////

// Declares a specific resourceBEGIN_RESOURCE_NO_PARENT
#define BEGIN_RESOURCE( ClassName, Extension, AccessMode )                    \
      BEGIN_RESOURCE_NO_PARENT( ClassName, Extension, AccessMode );           \
      PARENT( Resource );

///////////////////////////////////////////////////////////////////////////////

#define END_RESOURCE() END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCE_H 
