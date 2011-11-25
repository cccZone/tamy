#include "core-Renderer/SceneRenderTreeBuilder.h"
#include "core-Renderer/Geometry.h"
#include "core-Renderer/RenderState.h"


///////////////////////////////////////////////////////////////////////////////

GeometryNode::GeometryNode( GeometryNode*& nextNode, Geometry& geometry )
   : m_next( nextNode )
   , m_geometry( geometry )
{
   nextNode = this;
}

///////////////////////////////////////////////////////////////////////////////

GeometryNode::~GeometryNode()
{
   MEMPOOL_DELETE( m_next );
}

///////////////////////////////////////////////////////////////////////////////

void GeometryNode::render( Renderer& renderer ) const
{
   // render the geometry
   m_geometry.render( renderer );

   // and the one in the next node, if there's one
   if ( m_next )
   {
      m_next->render( renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

StateTreeNode::StateTreeNode() 
   : m_child( NULL )
   , m_sibling( NULL )
   , m_state( NULL ) 
   , m_managedObj( NULL )
   , m_geometryNode( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

StateTreeNode::StateTreeNode( RenderState& state ) 
   : m_child( NULL )
   , m_sibling( NULL )
   , m_state( &state ) 
   , m_managedObj( NULL )
   , m_geometryNode( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

StateTreeNode::StateTreeNode( RenderState* state, MemoryPoolObject* managedObj )
   : m_child( NULL )
   , m_sibling( NULL )
   , m_state( state ) 
   , m_managedObj( managedObj )
   , m_geometryNode( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

StateTreeNode::~StateTreeNode()
{
   MEMPOOL_DELETE( m_managedObj );
   MEMPOOL_DELETE( m_geometryNode );
   MEMPOOL_DELETE( m_child );
   MEMPOOL_DELETE( m_sibling );
}

///////////////////////////////////////////////////////////////////////////////

bool StateTreeNode::compareState( RenderState* state )
{
   if ( m_state == NULL )
   {
      // the node exist but it doesn't have a state assigned - so assign it 
      // the specified state and return true, since now it matches the state
      m_state = state;
      return true;
   }
   else
   {
      return m_state->equals( *state );
   }
}

///////////////////////////////////////////////////////////////////////////////

void StateTreeNode::render( Renderer& renderer ) const
{
   // first set the state
   if ( m_state )
   {
      m_state->onPreRender( renderer );
   }

   // then, if the node has children - render them - we want to go in depth
   if ( m_child )
   {
      m_child->render( renderer );
   }

   // now the entire state is set - render the geometry if there's any
   if ( m_geometryNode )
   {
      m_geometryNode->render( renderer );
   }

   // clear the state
   if ( m_state )
   {
      m_state->onPostRender( renderer );
   }

   // traverse the sibling node
   if ( m_sibling )
   {
      m_sibling->render( renderer );
   }

}

///////////////////////////////////////////////////////////////////////////////
