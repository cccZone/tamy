#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipeline.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/DefaultAttributeSorter.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/SpatialView.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RPStartNode.h"
#include "core/AABoundingBox.h"
#include "core-MVC/Model.h"
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
{
   // create the model views
   AABoundingBox sceneBB(D3DXVECTOR3( -10000, -10000, -10000 ), D3DXVECTOR3( 10000, 10000, 10000 ) );
   m_spatialView = new SpatialView( sceneBB );

   m_renderingView = new RenderingView();
   m_renderingView->setAttributeSorter( *m_statesManager );
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineMechanism::~RenderingPipelineMechanism()
{
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
      return m_pipeline->getRenderTarget( id );
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

      // initialize render targets
      const std::vector< RenderTargetDescriptor* >& renderTargets = m_pipeline->getRenderTargets();
      for ( std::vector< RenderTargetDescriptor* >::const_iterator it = renderTargets.begin(); it != renderTargets.end(); ++it )
      {
         (*it)->initialize( renderer );
      }

      // initialize nodes
      cacheNodes();
      for ( std::vector< RenderingPipelineNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
      {
         (*it)->initialize( *this );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::deinitialize()
{
   // deintialize nodes
   for ( std::vector< RenderingPipelineNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
   {
      (*it)->deinitialize( *this );
   }
   m_nodesQueue.clear();

   if ( m_pipeline )
   {
      // deinitialize render targets
      const std::vector< RenderTargetDescriptor* >& renderTargets = m_pipeline->getRenderTargets();
      for ( std::vector< RenderTargetDescriptor* >::const_iterator it = renderTargets.begin(); it != renderTargets.end(); ++it )
      {
         (*it)->deinitialize();
      }
   }
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

void RenderingPipelineMechanism::cacheNodes()
{
   m_nodesQueue.clear();

   if ( !m_pipeline )
   {
      return;
   }
   const std::vector< RenderingPipelineNode* >& nodes = m_pipeline->getNodes();

   // find the start node
   RenderingPipelineNode* startNode = NULL;
   for ( std::vector< RenderingPipelineNode* >::const_iterator it = nodes.begin(); it != nodes.end(); ++it )
   {
      startNode = dynamic_cast< RPStartNode* >( *it );
      if ( startNode != NULL )
      {
         break;
      }
   }
   if ( startNode == NULL )
   {
      return;
   }

   // a simple BFS algorithm that iterates through the graph 
   // and updates the nodes in turn
   std::list< RenderingPipelineNode* > nodesToCheck;
   nodesToCheck.push_back( startNode );

   while( !nodesToCheck.empty() )
   {
      RenderingPipelineNode* checkedNode = nodesToCheck.front();
      nodesToCheck.pop_front();

      // make sure the node's not already in the cache - we don't allow the same node to be updated twice
      // with this algorithm
      std::vector< RenderingPipelineNode* >::const_iterator checkIt = std::find( m_nodesQueue.begin(), m_nodesQueue.end(), checkedNode );
      if ( checkIt != m_nodesQueue.end() )
      {
         continue;
      }

      // all's clear - add the node to the queue and put its neighbors up for processing
      m_nodesQueue.push_back( checkedNode );

      std::vector< RenderingPipelineNode* > neighbors;
      checkedNode->getSubsequentNodes( neighbors );

      for ( std::vector< RenderingPipelineNode* >::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it )
      {
         nodesToCheck.push_back( *it );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
