/// @file   core-Renderer/RenderResourceStorage.h
/// @brief  storage for platform dependent resources
#ifndef _RENDER_RESOURCE_STORAGE_H
#define _RENDER_RESOURCE_STORAGE_H

#include "core\Delegate.h"
#include <vector>
#include <list>


///////////////////////////////////////////////////////////////////////////////

class Renderer;

///////////////////////////////////////////////////////////////////////////////

class IRenderResourceStorage
{
public:
   virtual ~IRenderResourceStorage() {}

   /**
    * Clears the storage, deleting all objects in it.
    */
   virtual void clear( const Renderer& renderer ) = 0;

   /**
    * Removes a render resource from the storage.
    *
    * @param resourceId
    */
   virtual void removeResource( int resourceId ) = 0;

   /**
    * Method called when the render device looses its context.
    */
   virtual void onDeviceLost() = 0;

   /**
    * Method called when the render device looses its context.
    */
   virtual void onDeviceRestored() = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Storage for platform dependent resources
 */
template< typename RENDERER, typename ENGINE_TYPE, typename RENDER_RESOURCE >
class RenderResourceStorage : public IRenderResourceStorage
{
private:
   const RENDERER&                  m_renderer;
   std::vector< ENGINE_TYPE* >      m_engineObjects;
   std::vector< RENDER_RESOURCE* >  m_resources;
   std::list< int >                 m_freeSlots;

public:
   /**
    * Constructor.
    *
    * @param renderer         host renderer
    */
   RenderResourceStorage( const RENDERER& renderer );
   ~RenderResourceStorage();

   /**
    * Returns an instance of a RenderResource assigned to the specified object.
    *
    * @param obj     engine renderer object
    */
   RENDER_RESOURCE* getInstance( ENGINE_TYPE& obj );

   // -------------------------------------------------------------------------
   // IRenderResourceStorage implementation
   // -------------------------------------------------------------------------
   void clear( const Renderer& renderer );
   void removeResource( int resourceId );
   void onDeviceLost();
   void onDeviceRestored();

private:

   /**
    * Creates a new instance of the resource.
    */
   RENDER_RESOURCE* createResource( const ENGINE_TYPE& obj ) const;

   /**
    * Releases the resource.
    */
   void releaseResource( RENDER_RESOURCE* resource ) const;

   /**
    * Informs the resource that the device context was lost.
    */
   void deviceLost( RENDER_RESOURCE* resource ) const;

   /**
    * Informs the resource that the device context was restored.
    */
   void deviceRestored( RENDER_RESOURCE* resource ) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\RenderResourceStorage.inl"

///////////////////////////////////////////////////////////////////////////////

#endif _RENDER_RESOURCE_STORAGE_H
