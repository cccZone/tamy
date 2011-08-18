#include "dx9-Renderer\DX9VertexShaderResourcesStorage.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template<>
DX9VertexShader* RenderResourceStorage< DX9Renderer, VertexShader, DX9VertexShader >::createResource( const VertexShader& obj ) const
{
   return new DX9VertexShader( m_renderer, obj );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, VertexShader, DX9VertexShader >::releaseResource( DX9VertexShader* resource ) const
{
   delete resource;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, VertexShader, DX9VertexShader >::deviceLost( DX9VertexShader* resource ) const
{
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, VertexShader, DX9VertexShader >::deviceRestored( DX9VertexShader* resource ) const
{
}

///////////////////////////////////////////////////////////////////////////////
