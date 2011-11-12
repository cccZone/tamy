#include "core-Renderer/RPSceneRenderNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/VoidSockets.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/SpatialRepresentation.h"


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
   if ( !host.isSceneActive( m_renderedSceneId ) )
   {
      // no scene - no rendering
      return;
   }

   RenderTarget* trg = host.data()[ m_renderTarget ];
   Renderer& renderer = host.getRenderer();
  
   RenderingView& sceneRenderer = host.getSceneRenderer( m_renderedSceneId );

   new ( renderer() ) RCActivateRenderTarget( trg );
   sceneRenderer.render( *this );
}

///////////////////////////////////////////////////////////////////////////////

// TODO: create the node in a memory pool - will be much faster

StateTreeNode* RPSceneRenderNode::buildStateTree( const Array< SpatialRepresentation* >& visibleElems ) const
{
   StateTreeNode* root = new StateTreeNode( NULL );

   unsigned int elemsCount = visibleElems.size();
   for ( unsigned int i = 0; i < elemsCount; ++i )
   {     
      Geometry& geometry = visibleElems[i]->getGeometry();
      const RenderStatesVec& states = visibleElems[i]->getRenderStates();
      // at this point the states are already sorted

      // states span into the tree depth - so each branch is a unique state,
      // and its leaves contain geometry to render

      // find the child node for the state of the added element
      StateTreeNode* currNode = root;
      unsigned int statesCount = states.size();

      for( unsigned int j = 0; j < statesCount; ++j )
      {
         RenderState* checkedState = states[j];

         // get the child of the current node, and if there's none - create one
         if ( currNode->m_child == NULL )
         {
            currNode->m_child = new StateTreeNode( NULL );
         }
         currNode = currNode->m_child;


         // check the siblings and look for a similar state
         StateTreeNode* prevNode = currNode;
         while ( currNode && currNode->compareState( checkedState ) == false )
         {
            prevNode = currNode;
            currNode = currNode->m_sibling;
         }

         if ( currNode == NULL )
         {
            // there is no node with a matching state - create a new one
            prevNode->m_sibling = new StateTreeNode( checkedState );
            currNode = prevNode->m_sibling;
         }
      }

      // there are no more states to check - add the geometry
      new GeometryNode( currNode->m_geometryNode, geometry );
   }

   return root;
}

///////////////////////////////////////////////////////////////////////////////
