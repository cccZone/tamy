#ifndef _RENDER_RESOURCE_STORAGE_H
#error "This file can only be included from RenderResourceStorage.h"
#else


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
   // release the resources
   unsigned int count = m_resources.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      releaseResource( m_resources[i] );
   }
   m_resources.clear();
}

///////////////////////////////////////////////////////////////////////////////

template< typename RENDERER, typename ENGINE_TYPE, typename RENDER_RESOURCE >
RENDER_RESOURCE* RenderResourceStorage< RENDERER, ENGINE_TYPE, RENDER_RESOURCE >::getInstance( ENGINE_TYPE& obj )
{
   RENDER_RESOURCE* resource = NULL;

   int rrId = obj.getRenderResourceId( m_renderer );
   if ( rrId < 0 )
   {
      // the resource is not initialized - create it
      rrId = m_resources.size();
      resource = createResource( obj );

      if ( resource != NULL )
      {
         m_resources.push_back( resource );
         obj.setRenderResourceId( m_renderer, rrId );
      }
   }
   else
   {
      resource = m_resources[rrId];
   }

   return resource;
}

///////////////////////////////////////////////////////////////////////////////

template< typename RENDERER, typename ENGINE_TYPE, typename RENDER_RESOURCE >
void RenderResourceStorage< RENDERER, ENGINE_TYPE, RENDER_RESOURCE >::onDeviceLost()
{
   unsigned int count = m_resources.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      deviceLost( m_resources[i] );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename RENDERER, typename ENGINE_TYPE, typename RENDER_RESOURCE >
void RenderResourceStorage< RENDERER, ENGINE_TYPE, RENDER_RESOURCE >::onDeviceRestored()
{
   unsigned int count = m_resources.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      deviceRestored( m_resources[i] );
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif _RENDER_RESOURCE_STORAGE_H
