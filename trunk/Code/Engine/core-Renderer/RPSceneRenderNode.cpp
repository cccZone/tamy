#include "core-Renderer/RPSceneRenderNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/VoidSockets.h"
#include "core-Renderer/TextureSockets.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSceneRenderNode, RenderingPipelineNode )
   PROPERTY_EDIT( "Rendered scene id", RPMSceneId, m_renderedSceneId );
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPSceneRenderNode::RPSceneRenderNode()
: m_renderedSceneId( RPS_Main )
{
   defineInput( new RPVoidInput( "Input" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   host.data().registerVar( m_renderTarget );
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onInitialize( RenderingPipelineMechanism& host ) const
{
   host.data()[ m_renderTarget ] = getOutput< RPTextureOutput >( "Output" ).getRenderTarget( host.data() );
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onDeinitialize( RenderingPipelineMechanism& host ) const
{
   host.data()[ m_renderTarget ] = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RenderTarget* trg = host.data()[ m_renderTarget ];
   host.renderScene( m_renderedSceneId, trg );
}

///////////////////////////////////////////////////////////////////////////////
