#ifndef _RENDER_RESOURCE_STORAGE_H
#error "This file can only be included from RenderResourceStorage.h"
#else

#include "core-Renderer/Renderer.h"


///////////////////////////////////////////////////////////////////////////////

template< typename RENDERER, typename ENGINE_TYPE, typename RENDER_RESOURCE >
RenderResourceStorage< RENDERER, ENGINE_TYPE, RENDER_RESOURCE >::RenderResourceStorage( const RENDERER& renderer )
   : m_renderer( renderer )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename RENDERER, typename ENGINE_TYPE, typename RENDER_RESOURCE >
RenderResourceStorage< RENDERER, ENGINE_TYPE, RENDER_RESOURCE >::~RenderResourceStorage()
{
   // for some reason type resolution doesn't work in this destructor, and I can't
   // call any method on the engine objects that accepts the 'm_renderer'.
   // That's why I created a method clear, which doesn't have that problem.
   // Call it before deleting the storage so that all the host engine objects know
   // their renderer implementations are being removed
}

///////////////////////////////////////////////////////////////////////////////

template< typename RENDERER, typename ENGINE_TYPE, typename RENDER_RESOURCE >
void RenderResourceStorage< RENDERER, ENGINE_TYPE, RENDER_RESOURCE >::clear( const Renderer& renderer )
{
   // release the resources
   unsigned int count = m_resources.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_resources[i] != NULL )
      {
         // inform the host render resource that it's implementation is being released
         m_engineObjects[i]->onRenderResourceReleased( renderer );

         // release the resource
         releaseResource( m_resources[i] );
      }
   }
   m_engineObjects.clear();
   m_resources.clear();
   m_freeSlots.clear();
}

///////////////////////////////////////////////////////////////////////////////

template< typename RENDERER, typename ENGINE_TYPE, typename RENDER_RESOURCE >
RENDER_RESOURCE* RenderResourceStorage< RENDERER, ENGINE_TYPE, RENDER_RESOURCE >::getInstance( ENGINE_TYPE& obj )
{
   RENDER_RESOURCE* resource = NULL;

   int rrId = obj.getRenderResourceId( m_renderer );
   if ( rrId < 0 )
   {
      // find the first free slot for the new resource
      if ( !m_freeSlots.empty() )
      {
         rrId = m_freeSlots.front();
         m_freeSlots.pop_front();
      }
      else
      {
         rrId = m_resources.size();
         m_engineObjects.push_back( NULL );
         m_resources.push_back( NULL );
      }
   }

   resource = m_resources[rrId];
   if ( resource == NULL )
   {
      // the resource is not initialized - create it
      resource = createResource( obj );

      if ( resource != NULL )
      {
         m_resources[rrId] = resource;
         m_engineObjects[rrId] = &obj;

         obj.setRenderResourceId( m_renderer, *this, rrId );
      }
   }

   return resource;
}

///////////////////////////////////////////////////////////////////////////////

template< typename RENDERER, typename ENGINE_TYPE, typename RENDER_RESOURCE >
void RenderResourceStorage< RENDERER, ENGINE_TYPE, RENDER_RESOURCE >::removeResource( int resourceId )
{
   if ( resourceId < 0 || resourceId >= (int)m_resources.size() )
   {
      return;
   }

   RENDER_RESOURCE* resource = m_resources[resourceId];
   delete resource;
   m_resources[resourceId] = NULL;
   m_engineObjects[resourceId] = NULL;
   m_freeSlots.push_back( resourceId );
}

///////////////////////////////////////////////////////////////////////////////

template< typename RENDERER, typename ENGINE_TYPE, typename RENDER_RESOURCE >
void RenderResourceStorage< RENDERER, ENGINE_TYPE, RENDER_RESOURCE >::onDeviceLost()
{
   unsigned int count = m_resources.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_resources[i] != NULL )
      {
         deviceLost( m_resources[i] );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename RENDERER, typename ENGINE_TYPE, typename RENDER_RESOURCE >
void RenderResourceStorage< RENDERER, ENGINE_TYPE, RENDER_RESOURCE >::onDeviceRestored()
{
   unsigned int count = m_resources.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_resources[i] != NULL )
      {
         deviceRestored( m_resources[i] );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif _RENDER_RESOURCE_STORAGE_H
