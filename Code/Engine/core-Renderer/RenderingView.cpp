#include "core-Renderer\RenderingView.h"
#include "core-MVC\Entity.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\SpatialRepresentation.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderState.h"

///////////////////////////////////////////////////////////////////////////////

RenderingView::RenderingView( Renderer& renderer, const AABoundingBox& sceneBB )
: m_renderer( renderer )
, m_storage( new RegularOctree< SpatialRepresentation >( sceneBB ) )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingView::~RenderingView()
{
   for ( SpatialsMap::iterator it = m_spatials.begin(); it != m_spatials.end(); ++it )
   {
      delete it->second;
   }
   m_spatials.clear();

   delete m_storage; m_storage = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::render()
{
   // tag visible objects
   const BoundingVolume& volume = m_renderer.getActiveCamera().getFrustum();

   Array< SpatialRepresentation* > visibleElems;
   m_storage->query( volume, visibleElems );

   // build a tree sorting the nodes by the attributes
   StateTreeNode* root = buildStateTree( visibleElems );
   
   // render the tree contents
   root->render( m_renderer );

   // get rid of the tree
   delete root;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::onEntityAdded( Entity& entity )
{
   // check if this is a geometry
   Geometry* geometry = dynamic_cast< Geometry* >( &entity );
   if ( geometry != NULL )
   {
      SpatialsMap::iterator it = m_spatials.find( geometry );
      if ( it == m_spatials.end() )
      {
         SpatialRepresentation* repr = new SpatialRepresentation( *geometry );
         m_spatials.insert( std::make_pair( geometry, repr ) );
         m_storage->insert( *repr );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::onEntityRemoved(Entity& entity)
{
   // check if this is a geometry
   Geometry* geometry = dynamic_cast< Geometry* >( &entity );
   if ( geometry != NULL )
   {
      SpatialsMap::iterator it = m_spatials.find( geometry );
      if ( it != m_spatials.end() )
      {
         m_storage->remove( *it->second );
         delete it->second;
         m_spatials.erase(it);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::onEntityChanged( Entity& entity )
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::resetContents()
{
   m_storage->clear();

   for ( SpatialsMap::iterator it = m_spatials.begin(); it != m_spatials.end(); ++it )
   {
      delete it->second;
   }
   m_spatials.clear();
}

///////////////////////////////////////////////////////////////////////////////

// TODO: create the node in a memory pool - will be much faster

RenderingView::StateTreeNode* RenderingView::buildStateTree( const Array< SpatialRepresentation* >& visibleElems ) const
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
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RenderingView::GeometryNode::GeometryNode( GeometryNode*& nextNode, Geometry& geometry )
   : m_next( nextNode )
   , m_geometry( geometry )
{
   nextNode = this;
}

///////////////////////////////////////////////////////////////////////////////

RenderingView::GeometryNode::~GeometryNode()
{
   delete m_next; m_next = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::GeometryNode::render( Renderer& renderer ) const
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

RenderingView::StateTreeNode::StateTreeNode( RenderState* state ) 
   : m_child( NULL )
   , m_sibling( NULL )
   , m_state( state ) 
   , m_geometryNode( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingView::StateTreeNode::~StateTreeNode()
{
   delete m_geometryNode; m_geometryNode = NULL;
   delete m_child; m_child = NULL;
   delete m_sibling; m_sibling = NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool RenderingView::StateTreeNode::compareState( RenderState* state )
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

void RenderingView::StateTreeNode::render( Renderer& renderer ) const
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
