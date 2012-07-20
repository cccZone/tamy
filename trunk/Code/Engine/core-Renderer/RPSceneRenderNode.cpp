#include "core-Renderer/RPSceneRenderNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/BasicRenderCommands.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/RPSBTextured.h"
#include "core/MemoryPool.h"


// TODO: !!!!!!!!!!!!!!!! MEGA WTOPA:
// SPRAWDZIC czy pozostale edytory nie beda sie crashowaly
// Wyjebac RPMSceneId i zwiazana z tym konfiguracje


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSceneRenderNode );
   PARENT( RenderingPipelineNode );
   PROPERTY_EDIT( "Rendered scene id", RPMSceneId, m_renderedSceneId );
   PROPERTY_EDIT( "Scene contents builder", RPSceneBuilder*, m_builder );
   PROPERTY_EDIT( "Render target id", std::string, m_renderTargetId );
   PROPERTY_EDIT( "Clear depth buffer", bool, m_clearDepthBuffer );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPSceneRenderNode::RPSceneRenderNode()
   : m_renderedSceneId( RPS_Main )
   , m_builder( NULL )
   , m_clearDepthBuffer( true )
   , m_renderTargetId( "Color" )
{
   defineInput( new RPVoidInput( "Input" ) );
   
   defineOutputs();
}
///////////////////////////////////////////////////////////////////////////////

RPSceneRenderNode::~RPSceneRenderNode()
{
   delete m_builder; m_builder = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_builder" && m_builder )
   {
      m_builder->defineSockets( *this );
   }

   if ( property.getName() == "m_renderTargetId" )
   {
      defineOutputs();
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::defineOutputs()
{
   std::vector< std::string > renderTargets;
   StringUtils::tokenize( m_renderTargetId, ";", renderTargets );

   // go through the existing sockets and create a list of sockets that correspond to removed render targets
   {
      std::vector< GBNodeOutput< RenderingPipelineNode >* > outputs = getOutputs();
      int outputsCount = outputs.size();
      for ( int outputIdx = outputsCount - 1; outputIdx >= 0; --outputIdx )
      {
         const std::string& outputName = outputs[outputIdx]->getName();
         bool stillExists = false;
         uint renderTargetsCount = renderTargets.size();
         for ( uint rtIdx = 0; rtIdx < renderTargetsCount; ++rtIdx )
         {
            if ( renderTargets[rtIdx] == outputName )
            {
               stillExists = true;
               renderTargets.erase( renderTargets.begin() + rtIdx );
               break;
            }
         }

         if ( stillExists )
         {
            outputs.erase( outputs.begin() + outputIdx );
         }
      }

      // remove the outputs that remain on the list
      outputsCount = outputs.size();
      for ( int outputIdx = outputsCount - 1; outputIdx >= 0; --outputIdx )
      {
         const std::string& outputName = outputs[outputIdx]->getName();
         this->removeOutput( outputName );
      }

      // now add new outputs
      uint renderTargetsCount = renderTargets.size();
      for ( uint rtIdx = 0; rtIdx < renderTargetsCount; ++rtIdx )
      {
         const std::string& rtName = renderTargets[rtIdx];
         defineOutput( new RPTextureOutput( rtName ) );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::refreshRenderTargets( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();
   Array< RenderTarget* >& renderTargets = *(data[ m_renderTargets ]);

   // remove old render targets
   renderTargets.clear();

   // acquire the new ones
   std::vector< GBNodeOutput< RenderingPipelineNode >* > renderTargetOutputs = getOutputs();
   uint count = renderTargetOutputs.size();
   renderTargets.resize( count );
   for ( uint i = 0; i < count; ++i )
   {
      RPTextureOutput* rtOutput = static_cast< RPTextureOutput* >( renderTargetOutputs[i] );
      const std::string& renderTargetName = rtOutput->getName();

      RenderTarget* target = host.getRenderTarget( renderTargetName );
      // we'll either find a target with the name we defined, or we're going to be rendering directly to back buffer.
      renderTargets[i] = target;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   // register runtime members
   data.registerVar( m_renderTargets );
   data.registerVar( m_treeMemPool );

   // create the render target
   {
      Array< RenderTarget* >* renderTargets = new Array< RenderTarget* >();
      data[ m_renderTargets ] = renderTargets;

      refreshRenderTargets( host );
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
   if ( !host.isSceneActive( m_renderedSceneId ) || !m_builder )
   {
      // no scene - no rendering
      return;
   }

   RuntimeDataBuffer& data = host.data();

   Array< RenderTarget* >& renderTargets = *(data[ m_renderTargets ]);
   Renderer& renderer = host.getRenderer();
   MemoryPool* treeMemPool = data[ m_treeMemPool ];

   // activate render targets we want to render to and set them on the outputs
   uint rtCount = renderTargets.size();
   for ( uint i = 0; i < rtCount; ++i )
   {
      new ( renderer() ) RCActivateRenderTarget( renderTargets[i] );
   }

   // collect the renderables
   const Array< Geometry* >& visibleElems = host.getSceneElements( m_renderedSceneId );

   // build a tree sorting the nodes by the attributes
   treeMemPool->reset();
   StateTreeNode* root = m_builder->buildRenderTree( *treeMemPool, visibleElems, data );

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
}

///////////////////////////////////////////////////////////////////////////////
