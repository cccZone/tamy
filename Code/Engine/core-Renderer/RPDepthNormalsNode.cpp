#include "core-Renderer/RPDepthNormalsNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/VoidSockets.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/RPSBComputed.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPDepthNormalsNode, RenderingPipelineNode )
   PROPERTY_EDIT( "Rendered scene id", RPMSceneId, m_renderedSceneId );
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPDepthNormalsNode::RPDepthNormalsNode()
   : m_renderedSceneId( RPS_Main )
   , m_builder( new RPSBComputed() )
{
   defineInput( new RPVoidInput( "Input" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

RPDepthNormalsNode::~RPDepthNormalsNode()
{
   delete m_builder; m_builder = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPDepthNormalsNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& rtData = host.data();
   rtData.registerVar( m_renderTarget );
}

///////////////////////////////////////////////////////////////////////////////

void RPDepthNormalsNode::onInitialize( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& rtData = host.data();
   rtData[ m_renderTarget ] = getOutput< RPTextureOutput >( "Output" ).getRenderTarget( rtData );
}

///////////////////////////////////////////////////////////////////////////////

void RPDepthNormalsNode::onDeinitialize( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& rtData = host.data();
   rtData[ m_renderTarget ] = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPDepthNormalsNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   if ( !host.isSceneActive( m_renderedSceneId ) )
   {
      // no scene - no rendering
      return;
   }

   RenderTarget* trg = host.data()[ m_renderTarget ];
   Renderer& renderer = host.getRenderer();

   RenderingView& sceneRenderer = host.getSceneRenderer( m_renderedSceneId );

   new ( renderer() ) RCActivateRenderTarget( trg );
   sceneRenderer.render( *m_builder );
}

///////////////////////////////////////////////////////////////////////////////
