#include "core-Renderer/RPDebugRendererNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderTarget.h"


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

   RenderTarget* trg = host.getRenderTarget( m_renderTargetId );
   data[ m_renderTarget ] = trg;

   // find the existing outputs and set the data on them
   RPTextureOutput* output = DynamicCast< RPTextureOutput >( findOutput( "Output" ) );
   output->setValue( data, trg );
}

///////////////////////////////////////////////////////////////////////////////

void RPDebugRendererNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RenderTarget* trg = host.data()[ m_renderTarget ];
   host.renderDebugScene( trg );
}

///////////////////////////////////////////////////////////////////////////////
