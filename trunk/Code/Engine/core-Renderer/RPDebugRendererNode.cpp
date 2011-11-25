#include "core-Renderer/RPDebugRendererNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/Renderer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPDebugRendererNode, RenderingPipelineNode )
   PROPERTY_EDIT( "Render target id", std::string, m_renderTargetId );
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPDebugRendererNode::RPDebugRendererNode()
{
   defineInput( new RPVoidInput( "Input" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPDebugRendererNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   data.registerVar( m_renderTarget );
   data[ m_renderTarget ] = &host.getRenderTarget( m_renderTargetId );
}

///////////////////////////////////////////////////////////////////////////////

void RPDebugRendererNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RenderTarget* trg = host.data()[ m_renderTarget ];
   host.renderDebugScene( trg );
}

///////////////////////////////////////////////////////////////////////////////
