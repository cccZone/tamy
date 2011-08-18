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
      StateTreeNode* currNode = root;
      unsigned int statesCount = states.size();
      for( unsigned int j = 0; j < statesCount;  )
      {
         if ( currNode->compareState( states[j] ) )
         {
            // state of this node matches that of the checked state - so go deeper with the tree exploration
            if ( currNode->m_child == NULL )
            {
               // create it if necessary
               currNode->m_child = new StateTreeNode( NULL ); 
            }
            currNode = currNode->m_child;
            ++j; // check the next state
         }
         else
         {
            // state of this node doesn't match the checked state - check the sibling, and keep checking
            // until you either find a match, or you need to create a brand new one
            while( currNode->m_sibling != NULL && currNode->m_sibling->compareState( states[j] ) )
            {
               currNode = currNode->m_sibling;
            }
            
            if ( currNode->m_sibling == NULL )
            {
               // ok - we went out of siblings - create a new one
               currNode->m_sibling = new StateTreeNode( states[j] );
               currNode = currNode->m_sibling;
            }
            
            ++j;
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
      return *m_state == *state;
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
