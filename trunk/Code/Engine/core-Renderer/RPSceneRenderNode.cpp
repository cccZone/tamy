#include "core-Renderer/RPSceneRenderNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/SpatialView.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/AttributeSorter.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/VoidSockets.h"
#include "core-Renderer/TextureSockets.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSceneRenderNode, RenderingPipelineNode )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPSceneRenderNode::RPSceneRenderNode()
: m_renderTarget( NULL )
{
   defineInput( new RPVoidInput( "Input" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onInitialize( RenderingPipelineMechanism& host )
{
   m_renderTarget = getOutput< RPTextureOutput >( "Output" ).getRenderTarget();
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onDeinitialize( RenderingPipelineMechanism& host )
{
   m_renderTarget = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onUpdate( RenderingPipelineMechanism& host )
{
   Renderer& renderer = host.getRenderer();
   renderer.setRenderTarget( m_renderTarget );

   host.getStatesMgr().render();
}

///////////////////////////////////////////////////////////////////////////////
