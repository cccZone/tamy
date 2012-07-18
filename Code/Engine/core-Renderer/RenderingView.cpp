#include "core-Renderer\RenderingView.h"
#include "core-MVC\Entity.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderState.h"
#include "core-Renderer\SceneRenderTreeBuilder.h"
#include "core\MemoryPool.h"


///////////////////////////////////////////////////////////////////////////////

RenderingView::RenderingView( Renderer& renderer, const AABoundingBox& sceneBB )
   : m_renderer( renderer )
   , m_treeMemPool( new MemoryPool( 1024 * 1024 ) )
   , m_geometryStorage( new RegularOctree< Geometry >( sceneBB ) )
   , m_lightsStorage( new RegularOctree< Light >( sceneBB ) )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingView::~RenderingView()
{
   delete m_geometryStorage; m_geometryStorage = NULL;
   delete m_lightsStorage; m_lightsStorage = NULL;
   delete m_treeMemPool; m_treeMemPool;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::collectRenderables( Array< Geometry* >& outVisibleElems )
{
   // tag visible objects
   const BoundingVolume& volume = m_renderer.getActiveCamera().getFrustum();

   m_geometryStorage->query( volume, outVisibleElems );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::collectLights( Array< Light* >& outVisibleLights )
{
   // tag visible objects
   const BoundingVolume& volume = m_renderer.getActiveCamera().getFrustum();

   m_lightsStorage->query( volume, outVisibleLights );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::onEntityAdded( Entity& entity )
{
   if ( entity.isA< Geometry >() )
   {
      m_geometryStorage->insert( static_cast< Geometry& >( entity ) );
   }
   else if ( entity.isA< Light >() )
   {
      m_lightsStorage->insert( static_cast< Light& >( entity ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::onEntityRemoved(Entity& entity)
{
   if ( entity.isA< Geometry >() )
   {
      m_geometryStorage->remove( static_cast< Geometry& >( entity ) );
   }
   else if ( entity.isA< Light >() )
   {
      m_lightsStorage->remove( static_cast< Light& >( entity ) );
   }

}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::onEntityChanged( Entity& entity )
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::resetContents()
{
   m_geometryStorage->clear();
   m_lightsStorage->clear();
}

///////////////////////////////////////////////////////////////////////////////
