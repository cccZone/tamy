#include "core-Renderer\RenderingView.h"
#include "core-MVC\Entity.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderState.h"
#include "core-Renderer\SceneRenderTreeBuilder.h"
#include "core\MemoryPool.h"


///////////////////////////////////////////////////////////////////////////////

RenderingView::RenderingView( Renderer& renderer, const AABoundingBox& sceneBB )
   : m_renderer( renderer )
   , m_treeMemPool( new MemoryPool( 1024 * 1024 ) )
   , m_storage( new RegularOctree< Geometry >( sceneBB ) )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingView::~RenderingView()
{
   delete m_storage; m_storage = NULL;
   delete m_treeMemPool; m_treeMemPool;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::collectRenderables( Array< Geometry* >& outVisibleElems )
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
      m_storage->insert( *geometry );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::onEntityRemoved(Entity& entity)
{
   // check if this is a geometry
   Geometry* geometry = dynamic_cast< Geometry* >( &entity );
   if ( geometry != NULL )
   {
      m_storage->remove( *geometry );
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
}

///////////////////////////////////////////////////////////////////////////////
