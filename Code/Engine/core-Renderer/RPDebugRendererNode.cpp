#include "core-Renderer/RPDebugRendererNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/VoidSockets.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/Renderer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPDebugRendererNode, RenderingPipelineNode )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPDebugRendererNode::RPDebugRendererNode()
{
   defineInput( new RPVoidInput( "Input" ) );
   defineOutput( new RPRenderTargetOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPDebugRendererNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   data.registerVar( m_renderTarget );
}

///////////////////////////////////////////////////////////////////////////////

void RPDebugRendererNode::onInitialize( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   data[ m_renderTarget ] = getOutput< RPRenderTargetOutput >( "Output" ).getRenderTarget( data );
}

///////////////////////////////////////////////////////////////////////////////

void RPDebugRendererNode::onDeinitialize( RenderingPipelineMechanism& host ) const
{
   host.data()[ m_renderTarget ] = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPDebugRendererNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RenderTarget* trg = host.data()[ m_renderTarget ];
   host.renderDebugScene( trg );
}

///////////////////////////////////////////////////////////////////////////////
