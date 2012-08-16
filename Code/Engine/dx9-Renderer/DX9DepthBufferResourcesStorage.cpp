#include "dx9-Renderer\DX9DepthBufferResourcesStorage.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template<>
DX9DepthBuffer* RenderResourceStorage< DX9Renderer, DepthBuffer, DX9DepthBuffer >::createResource( const DepthBuffer& obj ) const
{
   return new DX9DepthBuffer( m_renderer, obj );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, DepthBuffer, DX9DepthBuffer >::releaseResource( DX9DepthBuffer* resource ) const
{
   delete resource;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, DepthBuffer, DX9DepthBuffer >::deviceLost( DX9DepthBuffer* resource ) const
{
   if ( resource )
   {
      resource->onLostDevice();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, DepthBuffer, DX9DepthBuffer >::deviceRestored( DX9DepthBuffer* resource ) const
{
   if ( resource )
   {
      resource->onResetDevice();
   }
}

///////////////////////////////////////////////////////////////////////////////
