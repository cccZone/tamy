#pragma once

/// @file   core\Resource.h
/// @brief  interface marking all resources

#include "core\Object.h"
#include "core\Serializer.h"
#include "core\Component.h"


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * This interface marks all resources that can be loaded using 
 * the resources manager.
 */
class Resource : public Object
{
   DECLARE_CLASS( Resource )

public:
   enum AccessMode
   {
      AM_BINARY,
      AM_TEXT
   };

   friend class ResourcesManager;

private:
   std::string                   m_filePath;
   ResourcesManager*             m_host;

public:
   /**
    * Constructor.
    *
    * @param filePath   path to the file where we want to save the resource.
    */
   Resource( const std::string& filePath = std::string() );
   virtual ~Resource() {}

   /**
    * Returns the name of the resource.
    */
   inline const std::string& getResourceName() const{ return m_filePath; }

   /**
    * Sets a new path to the resource.
    *
    * @param path
    * @param host    host manager performing the operation
    */
   void setFilePath( const std::string& path, ResourcesManager* host = NULL );

   /**
    * Returns the path to the file holding this resource.
    */
   inline const std::string& getFilePath() const { return m_filePath; }

   /**
    * A helper method allowing to saves the resource to an associated file.
    *
    * @param outExternalDependencies   other resources used by this resource
    */
   void saveResource( ExternalDependenciesSet& outExternalDependencies = ExternalDependenciesSet() );

   /**
    * Tells whether the resource is managed by a resources manager.
    */
   inline bool isManaged() const { return m_host != NULL; }

   /**
    * Informs the resource that from now on it's managed by a resources manager.
    */
   void setResourcesManager( ResourcesManager& mgr );

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
   static Class findResourceClass( const std::string& extension );

   /**
    * Registers a new resource type.
    *
    * @param extension     resource file extension
    * @param accessMode    resource file access mode
    * @param typeName      resource type name
    */
   static void registerResource( const std::string& extension, 
                                 AccessMode accessMode, 
                                 const std::string& typeName );

protected:
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
    * the resource has successfully been registered with the manager.
    */
    virtual void onResourceLoaded(ResourcesManager& mgr) {}
};

///////////////////////////////////////////////////////////////////////////////

// Resource declaration
#define DECLARE_RESOURCE( ClassName )                                         \
   DECLARE_CLASS( ClassName )                                                 \
   public:                                                                    \
      static const char* getExtension();                                      \
      virtual const char* getVirtualExtension();                              \
   private:

///////////////////////////////////////////////////////////////////////////////

// Declares a specific resource
#define BEGIN_RESOURCE( ClassName, ParentClassName, Extension, AccessMode )   \
   class RegisterResource##Extension                                          \
   {                                                                          \
   public:                                                                    \
      RegisterResource##Extension()                                           \
      {                                                                       \
         TypeID< ClassName > type;                                            \
         Resource::registerResource( #Extension, Resource::AccessMode, type.name() ); \
      }                                                                       \
   };                                                                         \
   RegisterResource##Extension resourceTypeRegistryFor_##Extension;           \
   const char* ClassName::getExtension() { return #Extension; }               \
   const char* ClassName::getVirtualExtension() { return ClassName::getExtension(); } \
   BEGIN_OBJECT( ClassName, ParentClassName )

///////////////////////////////////////////////////////////////////////////////

#define END_RESOURCE() END_OBJECT()

///////////////////////////////////////////////////////////////////////////////
