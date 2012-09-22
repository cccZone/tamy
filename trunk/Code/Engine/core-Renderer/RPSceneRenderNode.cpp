#include "core-Renderer/RPSceneRenderNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/BasicRenderCommands.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/MRTUtil.h"
#include "core-Renderer/StatefulRenderTreeBuilder.h"
#include "core-Renderer/RenderTree.h"
#include "core/MemoryPool.h"



///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSceneRenderNode );
   PARENT( RenderingPipelineNode );
   PROPERTY_EDIT( "Render target ids", std::string, m_renderTargetId );
   PROPERTY_EDIT( "Clear depth buffer", bool, m_clearDepthBuffer );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPSceneRenderNode::RPSceneRenderNode()
   : m_clearDepthBuffer( true )
   , m_renderTargetId( "Color" )
{
   defineInput( new RPVoidInput( "Input" ) );
   
   std::vector< GBNodeOutput< RenderingPipelineNode >* > outputs;
   MRTUtil::createOutputs( m_renderTargetId, outputs );
   defineOutputs( outputs );
}
///////////////////////////////////////////////////////////////////////////////

RPSceneRenderNode::~RPSceneRenderNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_renderTargetId" )
   {
      std::vector< GBNodeOutput< RenderingPipelineNode >* > outputs;
      MRTUtil::createOutputs( m_renderTargetId, outputs );
      redefineOutputs( outputs );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   // register runtime members
   data.registerVar( m_renderTargets );
   data.registerVar( m_treeMemPool );

   // create render targets
   {
      Array< RenderTarget* >* renderTargets = new Array< RenderTarget* >();
      data[ m_renderTargets ] = renderTargets;

      MRTUtil::refreshRenderTargets( host, this, *renderTargets );
   }

   // create a memory pool for the render tree
   {
      MemoryPool* treeMemPool = new MemoryPool( 1024 * 1024 );
      data[ m_treeMemPool ] = treeMemPool;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onDestroyLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   Array< RenderTarget* >* renderTargets = data[ m_renderTargets ];
   delete renderTargets;

   MemoryPool* treeMemPool = data[ m_treeMemPool ];
   delete treeMemPool;
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   Array< RenderTarget* >& renderTargets = *(data[ m_renderTargets ]);
   if ( renderTargets.size() == 0 )
   {
      // nothing to render the scene to - bail
      return;
   }

   Renderer& renderer = host.getRenderer();
   MemoryPool* treeMemPool = data[ m_treeMemPool ];

   // activate render targets we want to render to
   uint rtCount = renderTargets.size();
   for ( uint i = 0; i < rtCount; ++i )
   {
      new ( renderer() ) RCActivateRenderTarget( renderTargets[i], i );
   }

   // collect the renderables
   const Array< Geometry* >& visibleElems = host.getSceneElements();

   // build a tree sorting the nodes by the attributes
   treeMemPool->reset();
   StateTreeNode* root = StatefulRenderTreeBuilder::buildRenderTree( *treeMemPool, visibleElems );

   if ( root )
   {
      if ( m_clearDepthBuffer )
      {
         new ( renderer() ) RCClearDepthBuffer();
      }

      // render the tree contents
      root->render( renderer );

      // get rid of the tree
      MEMPOOL_DELETE( root );
   }

   // unbind render targets
   for ( uint i = 0; i < rtCount; ++i )
   {
      new ( renderer() ) RCDeactivateRenderTarget( i );
   }

}

///////////////////////////////////////////////////////////////////////////////
