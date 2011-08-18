#include "core-Renderer\Skeleton.h"
#include "dx9-Renderer\DX9Renderer.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

void RCBindSkeleton::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   IDirect3DVertexBuffer9* vb = dxRenderer.getSkeletonVertexBuffer( m_skeleton );
   d3Device.SetStreamSource( 1, vb, 0, sizeof( VertexWeight ) );
}

///////////////////////////////////////////////////////////////////////////////

void RCUnbindSkeleton::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   d3Device.SetStreamSource( 1, NULL, 0, 0 );
}

///////////////////////////////////////////////////////////////////////////////
