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
