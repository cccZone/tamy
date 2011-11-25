#include "core-Renderer/RPSceneRenderNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/SpatialRepresentation.h"
#include "core-Renderer/RPSBTextured.h"
#include "core/MemoryPool.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSceneRenderNode, RenderingPipelineNode )
   PROPERTY_EDIT( "Rendered scene id", RPMSceneId, m_renderedSceneId );
   PROPERTY_EDIT( "Scene contents builder", RPSceneBuilder*, m_builder );
   PROPERTY_EDIT( "Render target id", std::string, m_renderTargetId );
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPSceneRenderNode::RPSceneRenderNode()
   : m_renderedSceneId( RPS_Main )
   , m_builder( NULL )
   , m_treeMemPool( new MemoryPool( 1024 * 1024 ) )
{
   defineInput( new RPVoidInput( "Input" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}
///////////////////////////////////////////////////////////////////////////////

RPSceneRenderNode::~RPSceneRenderNode()
{
   delete m_builder; m_builder = NULL;
   delete m_treeMemPool; m_treeMemPool = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onPropertyChanged( Property& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_builder" && m_builder )
   {
      m_builder->defineSockets( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onCreateLayout( RenderingPipelineMechanism& host ) const
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

void RPSceneRenderNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   if ( !host.isSceneActive( m_renderedSceneId ) || !m_builder )
   {
      // no scene - no rendering
      return;
   }

   RuntimeDataBuffer& data = host.data();
   RenderTarget* trg = data[ m_renderTarget ];
   Renderer& renderer = host.getRenderer();
  
   // activate the render target we want to render to
   new ( renderer() ) RCActivateRenderTarget( trg );

   // collect the renderables
   const Array< SpatialRepresentation* >& visibleElems = host.getSceneElements( m_renderedSceneId );

   // build a tree sorting the nodes by the attributes
   m_treeMemPool->reset();
   StateTreeNode* root = m_builder->buildRenderTree( *m_treeMemPool, visibleElems, data );

   if ( root )
   {
      // render the tree contents
      root->render( renderer );

      // get rid of the tree
      MEMPOOL_DELETE( root );
   }
}

///////////////////////////////////////////////////////////////////////////////
