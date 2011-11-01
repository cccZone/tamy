#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipeline.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/DebugDrawCommands.h"
#include "core/AABoundingBox.h"
#include "core-MVC/Model.h"
#include "core-MVC/ModelDebugScene.h"
#include "core/Graph.h"
#include "core/GraphAlgorithms.h"
#include "core/RuntimeData.h"
#include "core/Enum.h"
#include "core/IDebugDraw.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_ENUM( RPMSceneId );
   ENUM_VAL( RPS_Main );
   ENUM_VAL( RPS_Debug );
END_ENUM( RPMSceneId );

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineMechanism::RenderingPipelineMechanism( RenderingPipeline* pipeline )
   : m_pipeline( pipeline )
   , m_renderer( NULL )
   , m_runtimeDataBuffer( NULL )
   , m_debugScene( NULL )
{
   // attach self as the observer
   if ( m_pipeline )
   {
      m_pipeline->attachObserver( *this );
   }

   // initialize the scenes vector
   m_scenes.resize( RPS_MaxScenes );
   for ( int i = 0; i < RPS_MaxScenes; ++i )
   {
      m_scenes[i] = new RenderedScene();
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineMechanism::~RenderingPipelineMechanism()
{
   // detach the observer
   if ( m_pipeline )
   {
      m_pipeline->detachObserver( *this );
   }

   for ( int i = 0; i < RPS_MaxScenes; ++i )
   {
      delete m_scenes[i];
   }
   m_scenes.clear();

   m_debugScene = NULL;

   delete m_runtimeDataBuffer;
   m_runtimeDataBuffer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::addScene( RPMSceneId sceneId, Model& scene )
{
   ASSERT_MSG( ( unsigned int )sceneId < RPS_MaxScenes, "Trying to add a scene with an invalid sceneId" );
   if ( ( unsigned int )sceneId < RPS_MaxScenes )
   {
      // make sure we're not trying to set the same scene for two different ids by removing it
      removeScene( scene );

      // detach the listeners from the old scene
      removeScene( sceneId );

      // set the new scene
      m_scenes[sceneId]->setModel( &scene );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::removeScene( RPMSceneId sceneId )
{
   ASSERT_MSG( ( unsigned int )sceneId < RPS_MaxScenes, "Trying to add a scene with an invalid sceneId" );
   if ( ( unsigned int )sceneId < RPS_MaxScenes )
   {
      // remove the model
      m_scenes[sceneId]->setModel( NULL );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::removeScene( Model& scene )
{
   bool foundMatch = false;

   for( std::vector< RenderedScene* >::iterator it = m_scenes.begin(); it != m_scenes.end(); ++it )
   {
      if ( **it == scene )
      {
         // verify that the scene's not registered twice - that would prove the addition mechanism
         // that tries to remove the duplicates before registering the scene fails
         ASSERT_MSG( foundMatch == false, "Check the addition mechanism - it allowed for a scene to be registered under two distinct ids" );

         foundMatch = true;

         // remove the model
         (*it)->setModel( NULL );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::setDebugScene( DebugScene& debugScene )
{
   m_debugScene = &debugScene;

   // attach it to the models
   unsigned int count = m_scenes.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_scenes[i]->setDebugScene( debugScene );
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderTarget& RenderingPipelineMechanism::getRenderTarget( const std::string& id ) const
{
   if ( m_pipeline )
   {
      return m_pipeline->getRenderTarget( id, *m_runtimeDataBuffer );
   }
   else
   {
      throw std::runtime_error( "Rendering pipeline mechanism isn't properly initialized" );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::initialize( Renderer& renderer )
{
   ASSERT_MSG( m_renderer == NULL, "The mechanism is already initialized" );
   if ( m_renderer != NULL )
   {
      return;
   }

   // memorize the renderer instance
   m_renderer = &renderer;

   // initialize the scenes
   for( std::vector< RenderedScene* >::iterator it = m_scenes.begin(); it != m_scenes.end(); ++it )
   {
      (*it)->initialize( renderer );
   }

   if ( m_pipeline )
   {
      // first - initialize data layouts
      delete m_runtimeDataBuffer;
      m_runtimeDataBuffer = new RuntimeDataBuffer();
      const std::vector< RenderTargetDescriptor* >& renderTargets = m_pipeline->getRenderTargets();

      // initialize nodes
      cacheNodes();

      // 1.) create data layout
      for ( std::vector< RenderTargetDescriptor* >::const_iterator it = renderTargets.begin(); it != renderTargets.end(); ++it )
      {
         (*it)->createLayout( *m_runtimeDataBuffer );
      }
      for ( std::vector< RenderingPipelineNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
      {
         (*it)->createLayout( *this );
      }

      // 2.) initialize render targets and nodes
      for ( std::vector< RenderTargetDescriptor* >::const_iterator it = renderTargets.begin(); it != renderTargets.end(); ++it )
      {
         (*it)->initialize( *m_runtimeDataBuffer, renderer );
      }
      for ( std::vector< RenderingPipelineNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
      {
         (*it)->attachObserver( *this );
         (*it)->initialize( *this );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::deinitialize( Renderer& renderer )
{
   if ( m_renderer != &renderer )
   {
      return;
   }

   // deinitialize nodes
   for ( std::vector< RenderingPipelineNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
   {
      // some of the nodes might not be there - verify that
      (*it)->detachObserver( *this );
      (*it)->deinitialize( *this );
   }
   m_nodesQueue.clear();

   if ( m_pipeline )
   {
      // deinitialize render targets
      const std::vector< RenderTargetDescriptor* >& renderTargets = m_pipeline->getRenderTargets();
      for ( std::vector< RenderTargetDescriptor* >::const_iterator it = renderTargets.begin(); it != renderTargets.end(); ++it )
      {
         (*it)->deinitialize( *m_runtimeDataBuffer );
      }
   }

   // remove the runtime data buffer
   delete m_runtimeDataBuffer;
   m_runtimeDataBuffer = NULL;

   // reset the memorized renderer instance
   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::render( Renderer& renderer )
{
   if ( !m_pipeline )
   {
      return;
   }

   // render the pipeline
   new ( renderer() ) RCBeginScene();

   unsigned int count = m_nodesQueue.size();
   for( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->update( *this );
   }

   new ( renderer() ) RCEndScene();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::update( RenderingPipeline& subject )
{
   // do nothing - this is an initial update
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::update( RenderingPipeline& subject, const RenderingPipelineOperation& msg )
{
   if ( msg == RPO_PRE_CHANGE && m_renderer != NULL )
   {
      deinitialize( *m_renderer );
   }
   else if ( msg == RPO_POST_CHANGE && m_renderer != NULL )
   {
      initialize( *m_renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::update( RenderingPipelineNode& subject )
{
   // do nothing - this is an initial update
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::update( RenderingPipelineNode& subject, const RenderingPipelineNodeOperation& msg )
{
   if ( msg == RPNO_CHANGED && m_renderer != NULL )
   {
      deinitialize( *m_renderer );
   }

   if ( msg == RPNO_CHANGED && m_renderer != NULL )
   {
      initialize( *m_renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::cacheNodes()
{
   m_nodesQueue.clear();

   if ( !m_pipeline )
   {
      return;
   }
  
   RPGraph graph;
   m_pipeline->buildGraph( graph );

   std::vector< RPGraph::Index > sortedNodes;
   GraphTopologicalSort( sortedNodes, graph );

   for ( std::vector< RPGraph::Index >::const_iterator it = sortedNodes.begin(); it != sortedNodes.end(); ++it )
   {
      m_nodesQueue.push_back( graph.getNode( *it ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::renderScene( RPMSceneId sceneId, RenderTarget* renderTarget ) const
{
   if ( !m_renderer )
   {
      return;
   }

   ASSERT_MSG( ( unsigned int )sceneId < RPS_MaxScenes, "Trying to add a scene with an invalid sceneId" );
   if ( ( unsigned int )sceneId < RPS_MaxScenes )
   {
      new ( (*m_renderer)() ) RCActivateRenderTarget( renderTarget );
      m_scenes[ sceneId ]->render();
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::renderDebugScene( RenderTarget* renderTarget )
{
   if ( !m_renderer )
   {
      return;
   }

   // set the rendering target
   new ( (*m_renderer)() ) RCActivateRenderTarget( renderTarget );

   // prepare for debug scene rendering
   new ( (*m_renderer)() ) RCBeginDebugScene();

   // draw a reference grid
   drawGrid(  );

   // draw the debug info
   if ( m_debugScene != NULL )
   {
      m_debugScene->onDebugRender( *m_renderer );
   }

   // render the scene
   new ( (*m_renderer)() ) RCEndDebugScene();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::drawGrid() const
{
   const float spacing = 10;
   const float dim = 1000;
   float varPos;
   float boundPos = dim * spacing;
   static Color gridColor( 100, 100, 255 );

   for ( float i = -dim; i <= dim; ++i )
   {
      varPos = i * spacing;
      new ( (*m_renderer)() ) RCDrawLine( D3DXVECTOR3( -boundPos, 0, varPos ), D3DXVECTOR3( boundPos, 0, varPos ), gridColor );
      new ( (*m_renderer)() ) RCDrawLine( D3DXVECTOR3( varPos, 0, -boundPos ), D3DXVECTOR3( varPos, 0, boundPos ), gridColor );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RenderingPipelineMechanism::RenderedScene::RenderedScene()
   : m_renderingView( NULL )
   , m_debugSceneView( NULL )
   , m_model( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineMechanism::RenderedScene::~RenderedScene()
{
   delete m_renderingView;
   m_renderingView = NULL;

   delete m_debugSceneView;
   m_debugSceneView = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::RenderedScene::initialize( Renderer& renderer )
{
   // create the model views
   AABoundingBox sceneBB(D3DXVECTOR3( -10000, -10000, -10000 ), D3DXVECTOR3( 10000, 10000, 10000 ) );
   m_renderingView = new RenderingView( renderer, sceneBB );
}

///////////////////////////////////////////////////////////////////////////////

bool RenderingPipelineMechanism::RenderedScene::operator==( const Model& model ) const
{
   return m_model == &model;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::RenderedScene::setModel( Model* model )
{
   ASSERT_MSG( m_renderingView != NULL, "Rendering view doesn't exist" );
   if ( m_renderingView == NULL )
   {
      return;
   }

   if ( m_model )
   {
      // detach the old model from the views
      m_model->detach( *m_renderingView );

      if ( m_debugSceneView )
      {
         m_model->detach( *m_debugSceneView );
      }
   }

   // set the new model
   m_model = model;

   if ( m_model )
   {
      // attach the new model to the views
      m_model->attach( *m_renderingView );

      if ( m_debugSceneView )
      {
         m_model->attach( *m_debugSceneView );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::RenderedScene::setDebugScene( DebugScene& scene )
{
   ASSERT_MSG( m_renderingView != NULL, "Rendering view doesn't exist" );
   if ( m_renderingView == NULL )
   {
      return;
   }

   if ( m_model && m_debugSceneView )
   {
      m_model->detach( *m_debugSceneView );
   }

   delete m_debugSceneView;
   m_debugSceneView = new ModelDebugScene( scene );

   if ( m_model )
   {
      m_model->attach( *m_debugSceneView );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::RenderedScene::render()
{
   if ( m_renderingView )
   {
      m_renderingView->render();
   }
}

///////////////////////////////////////////////////////////////////////////////
