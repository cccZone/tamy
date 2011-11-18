#include "core-Renderer/RPSceneRenderNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/VoidSockets.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/SpatialRepresentation.h"
#include "core-Renderer/RPSBTextured.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSceneRenderNode, RenderingPipelineNode )
   PROPERTY_EDIT( "Rendered scene id", RPMSceneId, m_renderedSceneId );
   // PROPERTY_EDIT( "Scene contents builder", RPSceneBuilder*, m_builder );
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPSceneRenderNode::RPSceneRenderNode()
   : m_renderedSceneId( RPS_Main )
   , m_builder( new RPSBTextured() )
{
   defineInput( new RPVoidInput( "Input" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}
///////////////////////////////////////////////////////////////////////////////

RPSceneRenderNode::~RPSceneRenderNode()
{
   delete m_builder; m_builder = NULL;
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
   if ( !host.isSceneActive( m_renderedSceneId ) || !m_builder )
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
