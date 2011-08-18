#include "dx9-Renderer\DX9RenderTargetResourcesStorage.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template<>
DX9RenderTarget* RenderResourceStorage< DX9Renderer, RenderTarget, DX9RenderTarget >::createResource( const RenderTarget& obj ) const
{
   return new DX9RenderTarget( m_renderer, obj );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, RenderTarget, DX9RenderTarget >::releaseResource( DX9RenderTarget* resource ) const
{
   delete resource;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, RenderTarget, DX9RenderTarget >::deviceLost( DX9RenderTarget* resource ) const
{
   if ( resource )
   {
      resource->onLostDevice();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, RenderTarget, DX9RenderTarget >::deviceRestored( DX9RenderTarget* resource ) const
{
   if ( resource )
   {
      resource->onResetDevice();
   }
}

///////////////////////////////////////////////////////////////////////////////
