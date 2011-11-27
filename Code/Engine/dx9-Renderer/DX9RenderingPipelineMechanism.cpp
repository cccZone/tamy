#include "core-Renderer\RenderingPipelineMechanism.h"
#include "dx9-Renderer\DX9Renderer.h"


///////////////////////////////////////////////////////////////////////////////

void RCBeginScene::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   dxRenderer.beginScene();
}

///////////////////////////////////////////////////////////////////////////////

void RCEndScene::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   dxRenderer.endScene();
}

///////////////////////////////////////////////////////////////////////////////

void RCClearDepthBuffer::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   d3Device.Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
}

///////////////////////////////////////////////////////////////////////////////
