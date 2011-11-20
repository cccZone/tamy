#include "core-Renderer\RenderingView.h"
#include "core-MVC\Entity.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\SpatialRepresentation.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderState.h"
#include "core-Renderer\SceneRenderTreeBuilder.h"
#include "core\MemoryPool.h"


///////////////////////////////////////////////////////////////////////////////

RenderingView::RenderingView( Renderer& renderer, const AABoundingBox& sceneBB )
   : m_renderer( renderer )
   , m_treeMemPool( new MemoryPool( 1024 * 1024 ) )
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
   delete m_treeMemPool; m_treeMemPool;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::collectRenderables( Array< SpatialRepresentation* >& outVisibleElems )
{
   // tag visible objects
   const BoundingVolume& volume = m_renderer.getActiveCamera().getFrustum();

   m_storage->query( volume, outVisibleElems );
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
