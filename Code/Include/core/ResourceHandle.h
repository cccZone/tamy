/// @file   core/ResourceHandle.h
/// @brief  handle to an object aggregated in a resource.

#ifndef _RESOURCE_HANDLE_H
#define _RESOURCE_HANDLE_H

#include <string>
#include "core/ReflectionObject.h"


///////////////////////////////////////////////////////////////////////////////

class Resource;

///////////////////////////////////////////////////////////////////////////////

/**
 * An object managed by a resource.
 */
class ResourceObject : public ReflectionObject
{
   DECLARE_CLASS()

private:
   // resource object management system related data
   Resource*                     m_hostResource;      // host resource
   int                           m_objectId;          // id of an object in the resource

   // we're befriending a resource, because it can manage objects
   friend class Resource;

public:
   /**
    * Constructor.
    */
   ResourceObject();
   virtual ~ResourceObject() {}

   /**
    * Tells if the object is managed by a resource.
    */
   inline bool hasHostResource() const;

   /**
    * Returns the managing resource.
    */
   inline Resource& getHostResource();

   /**
    * Returns the object id in the managing resource.
    */
   inline int getObjectId();

protected:
   /**
    * Turns the object instance into an object managed by a resource.
    *
    * @param hostResource
    * @param objectId
    */
   void setHostResource( Resource& hostResource, int objectId );

   /**
   * Removes the object from being managed by a resource.
   */
   void resetHostResource();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Handle to an object aggregated in a resource.
 */
class ResourceHandle : public ReflectionObject
{
   DECLARE_CLASS()

protected:
   std::string          m_resourcePath;
   int                  m_objectId;

public:
   /**
    * Constructor.
    *
    * @param resourcePath        path to a parent resource of the object
    * @param objectId            id of an object in the resource
    */
   ResourceHandle( const std::string& resourcePath = "", int objectId = -1 );

   virtual ~ResourceHandle() {}

   void onObjectPreSave();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type specific version of a resource handle.
 */
template< typename T >
class TResourceHandle : public ResourceHandle
{
public:
   /**
    * Constructor.
    *
    * @param object
    */
   TResourceHandle( ResourceObject& object );

   /**
    * Constructor.
    *
    * @param resourcePath        path to a parent resource of the object
    * @param objectId            id of an object in the resource
    */
   TResourceHandle( const std::string& resourcePath = "", int objectId = -1 );

   /**
    * Access operator.
    */
   T& get();

   /**
    * Access operator ( const version )
    */
   const T& get() const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core/ResourceHandle.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCE_HANDLE_H
