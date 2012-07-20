#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipeline.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/BasicRenderCommands.h"
#include "core-Renderer/RPStartNode.h"
#include "core/AABoundingBox.h"
#include "core-MVC/Model.h"
#include "core/Graph.h"
#include "core/GraphAlgorithms.h"
#include "core/RuntimeData.h"
#include "core/ReflectionEnum.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

RenderingPipelineMechanism::RenderingPipelineMechanism( RenderingPipeline* pipeline )
   : m_pipeline( pipeline )
   , m_renderer( NULL )
   , m_runtimeDataBuffer( NULL )
   , m_scene( new RenderedScene() )
{
   // attach self as the observer
   if ( m_pipeline )
   {
      m_pipeline->attachObserver( *this );
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

   delete m_scene;
   m_scene = NULL;

   delete m_runtimeDataBuffer;
   m_runtimeDataBuffer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::setScene( Model* scene )
{
   m_scene->setModel( scene );
}

///////////////////////////////////////////////////////////////////////////////

const Array< Geometry*> & RenderingPipelineMechanism::getSceneElements() const 
{ 
   return m_scene->m_visibleElems; 
}

///////////////////////////////////////////////////////////////////////////////

const Array< Light*> & RenderingPipelineMechanism::getSceneLights() const 
{ 
   return m_scene->m_visibleLights; 
}

///////////////////////////////////////////////////////////////////////////////

bool RenderingPipelineMechanism::isRenderTargetDefined( const std::string& id ) const
{
   if ( m_pipeline )
   {
      RenderTargetDescriptor* rtDesc = m_pipeline->findRenderTarget( id );
      return rtDesc != NULL;
   }
   else
   {
      ASSERT_MSG( false, "Rendering pipeline mechanism isn't properly initialized" );
      return NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderTarget* RenderingPipelineMechanism::getRenderTarget( const std::string& id ) const
{
   if ( m_pipeline )
   {
      return m_pipeline->getRenderTarget( id, *m_runtimeDataBuffer );
   }
   else
   {
      ASSERT_MSG( false, "Rendering pipeline mechanism isn't properly initialized" );
      return NULL;
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

   // initialize the scene
   m_scene->initialize( *m_renderer );

   // initialize the pipeline
   pipelineInitialization();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::deinitialize( Renderer& renderer )
{
   if ( m_renderer != &renderer )
   {
      return;
   }

   pipelineDeinitialization();

   // deinitialize the scenes
   m_scene->deinitialize( *m_renderer );

   // reset the memorized renderer instance
   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::pipelineInitialization()
{
   ASSERT( m_renderer != NULL );

   if ( m_pipeline )
   {
      // first - initialize data layouts
      delete m_runtimeDataBuffer;
      m_runtimeDataBuffer = new RuntimeDataBuffer();
      const std::vector< RenderTargetDescriptor* >& renderTargets = m_pipeline->getRenderTargets();

      // initialize nodes
      cacheNodes();

      // 1.) initialize data the nodes may be precaching ( i.e. render targets )
      for ( std::vector< RenderTargetDescriptor* >::const_iterator it = renderTargets.begin(); it != renderTargets.end(); ++it )
      {
         (*it)->initialize( *m_runtimeDataBuffer, *m_renderer );
      }

      // 2.) initialize the nodes
      for ( std::vector< RenderingPipelineNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
      {
         (*it)->createLayout( *this );
      }

      // 3.) attach observers
      for ( std::vector< RenderingPipelineNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
      {
         (*it)->attachObserver( *this );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::pipelineDeinitialization()
{
   ASSERT( m_renderer != NULL );

   // 1.) detach observers
   for ( std::vector< RenderingPipelineNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
   {
      // some of the nodes might not be there - verify that
      (*it)->detachObserver( *this );
   }

   // 2.) deinitialize the nodes
   for ( std::vector< RenderingPipelineNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
   {
      // some of the nodes might not be there - verify that
      (*it)->destroyLayout( *this );
   }

   m_nodesQueue.clear();

   // deinitialize render targets
   if ( m_pipeline )
   {
      const std::vector< RenderTargetDescriptor* >& renderTargets = m_pipeline->getRenderTargets();
      for ( std::vector< RenderTargetDescriptor* >::const_iterator it = renderTargets.begin(); it != renderTargets.end(); ++it )
      {
         (*it)->deinitialize( *m_runtimeDataBuffer );
      }
   }

   // remove the runtime data buffer
   delete m_runtimeDataBuffer;
   m_runtimeDataBuffer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::render( Renderer& renderer )
{
   if ( !m_pipeline )
   {
      return;
   }

   // get visible geometry for all scenes
   m_scene->queryVisibleElements();

   // render the pipeline
   new ( renderer() ) RCBeginScene();

   uint count = m_nodesQueue.size();
   for( uint i = 0; i < count; ++i )
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

void RenderingPipelineMechanism::update( RenderingPipeline& subject, const GraphBuilderOperation& msg )
{
   if ( m_renderer != NULL )
   {
      if ( msg == GBO_PRE_CHANGE )
      {
         pipelineDeinitialization();
      }
      else if ( msg == GBO_POST_CHANGE  )
      {
         pipelineInitialization();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::update( RenderingPipelineNode& subject )
{
   // do nothing - this is an initial update
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::update( RenderingPipelineNode& subject, const GraphBuilderNodeOperation& msg )
{
   if ( m_renderer != NULL )
   {
      pipelineDeinitialization();
      pipelineInitialization();
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
  
   Graph< RenderingPipelineNode* > graph;
   m_pipeline->buildGraph< RPStartNode >( graph );

   std::vector< Graph< RenderingPipelineNode* >::Index > sortedNodes;
   GraphTopologicalSort( sortedNodes, graph );

   for ( std::vector< Graph< RenderingPipelineNode* >::Index >::const_iterator it = sortedNodes.begin(); it != sortedNodes.end(); ++it )
   {
      m_nodesQueue.push_back( graph.getNode( *it ) );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RenderingPipelineMechanism::RenderedScene::RenderedScene()
   : m_renderingView( NULL )
   , m_model( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineMechanism::RenderedScene::~RenderedScene()
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::RenderedScene::initialize( Renderer& renderer )
{
   // create the model views
   AABoundingBox sceneBB( Vector( -10000, -10000, -10000 ), Vector( 10000, 10000, 10000 ) );
   m_renderingView = new RenderingView( renderer, sceneBB );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::RenderedScene::deinitialize( Renderer& renderer )
{
   delete m_renderingView;
   m_renderingView = NULL;
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
   }

   // set the new model
   m_model = model;

   if ( m_model )
   {
      // attach the new model to the views
      m_model->attach( *m_renderingView );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::RenderedScene::queryVisibleElements()
{
   // query the visible geometry
   m_visibleElems.clear();
   m_renderingView->collectRenderables( m_visibleElems );

   // query visible lights
   m_visibleLights.clear();
   m_renderingView->collectLights( m_visibleLights );
}

///////////////////////////////////////////////////////////////////////////////
