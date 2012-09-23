#include "EditorDebugRenderer.h"
#include "core-Renderer/StatefulRenderTreeBuilder.h"
#include "core-Renderer/RenderTree.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/BasicRenderCommands.h"
#include "core-MVC.h"
#include "core.h"
#include "DebugEntitiesManager.h"


///////////////////////////////////////////////////////////////////////////////

EditorDebugRenderer::EditorDebugRenderer()
   : m_host( NULL )
   , m_renderTreeBuilder( new StatefulRenderTreeBuilder() )
   , m_treeMemPool( NULL )
   , m_debugScene( NULL )
   , m_renderingView( NULL )
   , m_visibleElems( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

EditorDebugRenderer::~EditorDebugRenderer()
{
   delete m_renderTreeBuilder;
   m_renderTreeBuilder = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void EditorDebugRenderer::setHost( DebugEntitiesManager& host )
{
   m_host = &host;
}

///////////////////////////////////////////////////////////////////////////////

void EditorDebugRenderer::add( Entity& entity )
{
   if ( m_debugScene )
   {
      m_debugScene->add( &entity, false );
   }
}

///////////////////////////////////////////////////////////////////////////////

void EditorDebugRenderer::remove( Entity& entity )
{
   if ( m_debugScene )
   {
      m_debugScene->remove( entity );
   }
}

///////////////////////////////////////////////////////////////////////////////

void EditorDebugRenderer::attachSceneView( ModelView& view )
{
   // don't allow duplicated views
   uint count = m_externalSceneViews.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_externalSceneViews[i] == &view )
      {
         return;
      }
   }
   // memorize the view
   m_externalSceneViews.push_back( &view );

   // if the scene already exists - attach the view
   if ( m_debugScene )
   {
      m_debugScene->attach( view );
   }
}

///////////////////////////////////////////////////////////////////////////////

void EditorDebugRenderer::detachSceneView( ModelView& view )
{
   // remove the view
   uint count = m_externalSceneViews.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_externalSceneViews[i] == &view )
      {
         m_externalSceneViews.remove( i );
         break;
      }
   }

   // and detach it from the scene if it's already created
   if ( m_debugScene )
   {
      m_debugScene->detach( view );
   }
}

///////////////////////////////////////////////////////////////////////////////

void EditorDebugRenderer::initialize( Renderer& renderer )
{
   // create the debug scene
   {
      m_debugScene = new Model();

      // create the rendering view and attach it to the debug scene
      AABoundingBox sceneBB( Vector( -10000, -10000, -10000 ), Vector( 10000, 10000, 10000 ) );
      m_renderingView = new RenderingView( renderer, sceneBB );
      m_debugScene->attach( *m_renderingView );

      // create an array that will hold visible geometry for rendering
      m_visibleElems = new GeometryArray();
   }

   // create a memory pool for the render tree
   m_treeMemPool = new MemoryPool( 1024 * 1024 );

   // attach external views
   uint count = m_externalSceneViews.size();
   for ( uint i = 0; i < count; ++i )
   {
      m_debugScene->attach( *m_externalSceneViews[i] );
   }

   // inform the host that the scene's been created and that it can add stuff to it
   if ( m_host )
   {
      m_host->onDebugSceneCreated();
   }
}

///////////////////////////////////////////////////////////////////////////////

void EditorDebugRenderer::deinitialize( Renderer& renderer )
{
   // inform the host that the scene's been created and that it can add stuff to it
   if ( m_host )
   {
      m_host->onDebugSceneDestroyed();
   }

   // detach external views
   uint count = m_externalSceneViews.size();
   for ( uint i = 0; i < count; ++i )
   {
      m_debugScene->detach( *m_externalSceneViews[i] );
   }

   delete m_treeMemPool;
   m_treeMemPool = NULL;

   delete m_debugScene;
   m_debugScene = NULL;

   delete m_renderingView;
   m_renderingView = NULL;

   delete m_visibleElems;
   m_visibleElems = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void EditorDebugRenderer::render( Renderer& renderer )
{
   new ( renderer() ) RCBeginScene();

   // render directly to the back buffer
   new ( renderer() ) RCActivateRenderTarget( NULL );

   // set common matrices
   Camera& camera = renderer.getActiveCamera();
   const Matrix& projectionMtx = camera.getProjectionMtx();
   const Matrix& viewMtx = camera.getViewMtx();
   new ( renderer() ) RCSetCameraMatrices( viewMtx, projectionMtx );

   // collect visible debug elements
   m_visibleElems->clear();
   m_renderingView->collectRenderables( *m_visibleElems );

   // build a tree sorting the nodes by the attributes
   m_treeMemPool->reset();
   StateTreeNode* root = StatefulRenderTreeBuilder::buildRenderTree( *m_treeMemPool, *m_visibleElems );

   if ( root )
   {
      // render the tree contents
      root->render( renderer );

      // get rid of the tree
      MEMPOOL_DELETE( root );
   }

   new ( renderer() ) RCEndScene();
}

///////////////////////////////////////////////////////////////////////////////
