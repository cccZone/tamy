#include "core-Renderer/RPDebugRendererNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/RPSBTextured.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/BasicRenderCommands.h"
#include "core-MVC/Model.h"
#include "core-MVC/ModelDebugScene.h"
#include "core/MemoryPool.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPDebugRendererNode );
   PARENT( RenderingPipelineNode );
   PROPERTY_EDIT( "Render target id", std::string, m_renderTargetId );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPDebugRendererNode::RPDebugRendererNode()
   : m_builder( NULL )
{
   defineInput( new RPVoidInput( "Input" ) );
   defineOutput( new RPTextureOutput( "Output" ) );

   // create a scene builder
   m_builder = new RPSBTextured();
}

///////////////////////////////////////////////////////////////////////////////

RPDebugRendererNode::~RPDebugRendererNode()
{
   delete m_builder; m_builder = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPDebugRendererNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   // register runtime variables
   data.registerVar( m_renderTarget );
   data.registerVar( m_debugScene );
   data.registerVar( m_debugSceneView );
   data.registerVar( m_renderingView );
   data.registerVar( m_visibleElems );
   data.registerVar( m_treeMemPool );

   // create the render target
   RenderTarget* trg = NULL;
   {
      trg = host.getRenderTarget( m_renderTargetId );
      data[ m_renderTarget ] = trg;
   }

   // find the existing outputs and set the data on them
   {
      RPTextureOutput* output = DynamicCast< RPTextureOutput >( findOutput( "Output" ) );
      output->setValue( data, trg );
   }

   // create the debug scene
   {
      Model* debugScene = new Model();
      ModelDebugScene* debugSceneView = new ModelDebugScene( *debugScene );

      // create the rendering view and attach it to the debug scene
      AABoundingBox sceneBB( Vector( -10000, -10000, -10000 ), Vector( 10000, 10000, 10000 ) );
      Renderer& renderer = host.getRenderer();
      RenderingView* renderingView = new RenderingView( renderer, sceneBB );
      debugScene->attach( *renderingView );

      // create an array that will hold visible geometry for rendering
      GeometryArray* visibleElems = new GeometryArray();

      // memorize the runtime instances
      data[ m_debugScene ] = debugScene;
      data[ m_debugSceneView ] = debugSceneView;
      data[ m_renderingView ] = renderingView;
      data[ m_visibleElems ] = visibleElems;
   }

   // create a memory pool for the render tree
   {
      MemoryPool* treeMemPool = new MemoryPool( 1024 * 1024 );
      data[ m_treeMemPool ] = treeMemPool;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPDebugRendererNode::onDestroyLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   MemoryPool* treeMemPool = data[ m_treeMemPool ];
   delete treeMemPool;

   GeometryArray* visibleElems = data[ m_visibleElems ];
   delete visibleElems;

   RenderingView* renderingView = data[ m_renderingView ];
   delete renderingView;

   ModelDebugScene* debugSceneView = data[ m_debugSceneView ];
   delete debugSceneView;

   Model* debugScene = data[ m_debugScene ];
   delete debugScene;
}

///////////////////////////////////////////////////////////////////////////////

void RPDebugRendererNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   Renderer& renderer = host.getRenderer();
   RuntimeDataBuffer& data = host.data();
   RenderTarget* renderTarget = data[ m_renderTarget ];
   GeometryArray* visibleElems = data[ m_visibleElems ];
   RenderingView* renderingView = data[ m_renderingView ];
   MemoryPool* treeMemPool = data[ m_treeMemPool ];

   // set the rendering target
   new ( renderer() ) RCActivateRenderTarget( renderTarget );

   // set common matrices
   Camera& camera = renderer.getActiveCamera();
   const Matrix& projectionMtx = camera.getProjectionMtx();
   const Matrix& viewMtx = camera.getViewMtx();
   new ( renderer() ) RCSetCameraMatrices( viewMtx, projectionMtx );

   // collect visible debug elements
   visibleElems->clear();
   renderingView->collectRenderables( *visibleElems );

   // build a tree sorting the nodes by the attributes
   treeMemPool->reset();
   StateTreeNode* root = m_builder->buildRenderTree( *treeMemPool, *visibleElems, data );

   if ( root )
   {
      // render the tree contents
      root->render( renderer );

      // get rid of the tree
      MEMPOOL_DELETE( root );
   }

   // <gizmo.todo> move this code to the editor debug renderer
/*
   // prepare for debug scene rendering
   new ( renderer() ) RCBeginDebugScene();

   // draw a reference grid
   static float gridLinesSpacing = 10;
   static float gridSize = 1000;
   static Color gridColor( 100, 100, 255 );
   new ( renderer() ) RCDrawDebugGrid( gridSize, gridLinesSpacing, gridColor );

   // draw the debug info
   if ( m_debugScene != NULL )
   {
      m_debugScene->onDebugRender( *m_renderer );
   }

   // render the scene
   new ( (*m_renderer)() ) RCEndDebugScene();*/
}

///////////////////////////////////////////////////////////////////////////////
