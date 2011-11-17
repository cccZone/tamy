#include "core-Renderer/RPDepthNormalsNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/VoidSockets.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/SpatialRepresentation.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/Defines.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPDepthNormalsNode, RenderingPipelineNode )
   PROPERTY_EDIT( "Rendered scene id", RPMSceneId, m_renderedSceneId );
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPDepthNormalsNode::RPDepthNormalsNode()
   : m_renderedSceneId( RPS_Main )
{
   defineInput( new RPVoidInput( "Input" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPDepthNormalsNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& rtData = host.data();
   rtData.registerVar( m_renderTarget );
   rtData.registerVar( m_renderState );
}

///////////////////////////////////////////////////////////////////////////////

void RPDepthNormalsNode::onInitialize( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& rtData = host.data();
   rtData[ m_renderTarget ] = getOutput< RPTextureOutput >( "Output" ).getRenderTarget( rtData );
   rtData[ m_renderState ] = new DepthNormalsRenderState();
}

///////////////////////////////////////////////////////////////////////////////

void RPDepthNormalsNode::onDeinitialize( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& rtData = host.data();
   rtData[ m_renderTarget ] = NULL;
   
   delete rtData[ m_renderState ];
   rtData[ m_renderState ] = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPDepthNormalsNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   if ( !host.isSceneActive( m_renderedSceneId ) )
   {
      // no scene - no rendering
      return;
   }

   RuntimeDataBuffer& rtData = host.data();
   RenderTarget* trg = rtData[ m_renderTarget ];
   Renderer& renderer = host.getRenderer();

   RenderingView& sceneRenderer = host.getSceneRenderer( m_renderedSceneId );

   new ( renderer() ) RCActivateRenderTarget( trg );
   DepthNormalsRenderState* builder = rtData[ m_renderState ];
   sceneRenderer.render( *builder );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RPDepthNormalsNode::DepthNormalsRenderState::DepthNormalsRenderState()
   : m_shader( NULL )
{
   ResourcesManager& mgr = ResourcesManager::getInstance();

   Resource& psRes = mgr.create( SHADERS_DIR "RenderingPipeline/DepthNormals.tpsh" );
   m_shader = DynamicCast< PixelShader >( &psRes );
}

///////////////////////////////////////////////////////////////////////////////

void RPDepthNormalsNode::DepthNormalsRenderState::onPreRender( Renderer& renderer )
{
   RCBindPixelShader* comm = new ( renderer() ) RCBindPixelShader( *m_shader );

   Camera& activeCam = renderer.getActiveCamera();
   comm->setFloat( "g_FarZ", activeCam.getFarClippingPlane() );
}

///////////////////////////////////////////////////////////////////////////////

void RPDepthNormalsNode::DepthNormalsRenderState::onPostRender( Renderer& renderer )
{
   new ( renderer() ) RCUnbindPixelShader( *m_shader );
}

///////////////////////////////////////////////////////////////////////////////

StateTreeNode* RPDepthNormalsNode::DepthNormalsRenderState::buildStateTree( const Array< SpatialRepresentation* >& visibleElems ) const
{
   // ignore all render states - just gather the geometry under one state node that will
   // set our pixel shader that will render the depth and normals

   StateTreeNode* root = new StateTreeNode( const_cast< DepthNormalsRenderState* >( this ) );
   GeometryNode** currGeometryNode = &(root->m_geometryNode);

   unsigned int elemsCount = visibleElems.size();
   for ( unsigned int i = 0; i < elemsCount; ++i )
   {     
      Geometry& geometry = visibleElems[i]->getGeometry();
      *currGeometryNode = new GeometryNode( *currGeometryNode, geometry );
   }

   return root;
}

///////////////////////////////////////////////////////////////////////////////
