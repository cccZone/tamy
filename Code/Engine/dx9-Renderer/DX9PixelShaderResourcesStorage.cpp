#include "dx9-Renderer\DX9PixelShaderResourcesStorage.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template<>
DX9PixelShader* RenderResourceStorage< DX9Renderer, PixelShader, DX9PixelShader >::createResource( const PixelShader& obj ) const
{
   return new DX9PixelShader( m_renderer, obj );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, PixelShader, DX9PixelShader >::releaseResource( DX9PixelShader* resource ) const
{
   delete resource;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, PixelShader, DX9PixelShader >::deviceLost( DX9PixelShader* resource ) const
{
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, PixelShader, DX9PixelShader >::deviceRestored( DX9PixelShader* resource ) const
{
}

///////////////////////////////////////////////////////////////////////////////
