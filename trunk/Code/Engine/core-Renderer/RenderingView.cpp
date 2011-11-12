#include "core-Renderer\RenderingView.h"
#include "core-MVC\Entity.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\SpatialRepresentation.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderState.h"
#include "core-Renderer\SceneRenderTreeBuilder.h"


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

void RenderingView::render( const SceneRenderTreeBuilder& treeBuilder )
{
   // tag visible objects
   const BoundingVolume& volume = m_renderer.getActiveCamera().getFrustum();

   Array< SpatialRepresentation* > visibleElems;
   m_storage->query( volume, visibleElems );

   // build a tree sorting the nodes by the attributes
   StateTreeNode* root = treeBuilder.buildStateTree( visibleElems );
   
   if ( root )
   {
      // render the tree contents
      root->render( m_renderer );

      // get rid of the tree
      delete root;
   }
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
