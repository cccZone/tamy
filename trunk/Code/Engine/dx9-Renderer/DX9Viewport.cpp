#include "core-Renderer\Viewport.h"
#include "dx9-Renderer\DX9Renderer.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

void RCSetViewport::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   d3Device.SetViewport( (const D3DVIEWPORT9* )&m_viewport );
}

///////////////////////////////////////////////////////////////////////////////

void RCResetViewport::execute( Renderer& renderer )
{
   Viewport viewport;
   renderer.getViewport( viewport );

   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   d3Device.SetViewport( (const D3DVIEWPORT9* )&viewport );
}

///////////////////////////////////////////////////////////////////////////////
