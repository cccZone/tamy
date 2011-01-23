#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipeline.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/DefaultAttributeSorter.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/SpatialView.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core/AABoundingBox.h"
#include "core-MVC/Model.h"
#include "core/Graph.h"
#include "core/GraphAlgorithms.h"
#include "core/RuntimeData.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RTTI( RenderingPipelineMechanism )
END_RTTI

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineMechanism::RenderingPipelineMechanism( RenderingPipeline* pipeline )
   : m_pipeline( pipeline )
   , m_cameraContext( NULL )
   , m_spatialView( NULL )
   , m_renderingView( NULL )
   , m_statesManager( new DefaultAttributeSorter() )
   , m_renderer( NULL )
   , m_runtimeDataBuffer( NULL )
{
   // create the model views
   AABoundingBox sceneBB(D3DXVECTOR3( -10000, -10000, -10000 ), D3DXVECTOR3( 10000, 10000, 10000 ) );
   m_spatialView = new SpatialView( sceneBB );

   m_renderingView = new RenderingView();
   m_renderingView->setAttributeSorter( *m_statesManager );

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

   m_renderer = NULL;

   deinitialize();

   delete m_cameraContext;
   m_cameraContext = NULL;

   delete m_statesManager;
   m_statesManager = NULL;

   delete m_spatialView;
   m_spatialView = NULL;

   delete m_renderingView;
   m_renderingView = NULL;

   delete m_runtimeDataBuffer;
   m_runtimeDataBuffer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::addScene( Model& scene )
{
   // check if the scene is there, and if it is - don't add it twice
   std::vector< Model* >::const_iterator it = std::find( m_scenes.begin(), m_scenes.end(), &scene );
   if ( it == m_scenes.end() )
   {
      m_scenes.push_back( &scene );
   }

   // notify the pipeline that the runtime params have changed
   scene.attach( *m_spatialView );
   scene.attach( *m_renderingView );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::removeScene( Model& scene )
{
   scene.detach( *m_spatialView );
   scene.detach( *m_renderingView );

   std::vector< Model* >::const_iterator it = std::find( m_scenes.begin(), m_scenes.end(), &scene );
   if ( it != m_scenes.end() )
   {
      m_scenes.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::setCamera( Camera& camera )
{
   delete m_cameraContext;
   m_cameraContext = new PlainCameraContext( camera );
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
   // implement the pass
   m_renderer = &renderer;
   renderer.implement< RenderingPipelineMechanism >( *this );

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

void RenderingPipelineMechanism::deinitialize()
{
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
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::render()
{
   if ( !m_renderer || !m_pipeline || !m_cameraContext )
   {
      return;
   }

   // run through the scene visibility
   m_spatialView->update( *m_cameraContext );

   // render the pipeline
   impl().passBegin();

   unsigned int count = m_nodesQueue.size();
   for( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->update( *this );
   }

   impl().passEnd();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::update( RenderingPipeline& subject )
{
   // do nothing - this is an initial update
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::update( RenderingPipeline& subject, const RenderingPipelineOperation& msg )
{
   if ( msg == RPO_PRE_CHANGE )
   {
      deinitialize();
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
      deinitialize();
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

