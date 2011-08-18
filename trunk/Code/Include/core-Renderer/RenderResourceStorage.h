/// @file   core-Renderer/RenderResourceStorage.h
/// @brief  storage for platform dependent resources
#ifndef _RENDER_RESOURCE_STORAGE_H
#define _RENDER_RESOURCE_STORAGE_H

#include "core\Delegate.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class IRenderResourceStorage
{
public:
   virtual ~IRenderResourceStorage() {}

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
   std::vector< RENDER_RESOURCE* >  m_resources;

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
