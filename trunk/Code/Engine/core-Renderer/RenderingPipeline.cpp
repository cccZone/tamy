#include "core-Renderer/RenderingPipeline.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RPStartNode.h"
#include <algorithm>
#include "core/Algorithms.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( RenderingPipeline, Resource, trp, AM_BINARY );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_nodes );
   PROPERTY( std::vector< RenderTargetDescriptor* >, m_renderTargets );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

RenderingPipeline::RenderingPipeline( const std::string& fileName )
   : Resource( fileName )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipeline::~RenderingPipeline()
{
   m_nodes.clear();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::addNode( RenderingPipelineNode* node )
{
   notify( RPO_PRE_CHANGE );

   addObject( node );
   m_nodes.push_back( node );
   
   notify( RPO_POST_CHANGE );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::removeNode( RenderingPipelineNode& node )
{
   std::vector< RenderingPipelineNode* >::iterator it = std::find( m_nodes.begin(), m_nodes.end(), &node );
   if ( it != m_nodes.end() )
   {
      notify( RPO_PRE_CHANGE );

      m_nodes.erase( it );
      removeObject( node.getObjectId() );

      notify( RPO_POST_CHANGE );
   }
   else
   {
      ASSERT_MSG( false, "Trying to remove a non-existent rendering pipeline node" );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::buildGraph( RPGraph& outGraph ) const
{
   // add all nodes to the graph, and find the start node while you're at it
   RenderingPipelineNode* startNode = NULL;
   for ( std::vector< RenderingPipelineNode* >::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it )
   {
      if ( startNode == NULL )
      {
         startNode = dynamic_cast< RPStartNode* >( *it );
      }

      outGraph.addNode( *it );
   }
   if ( startNode == NULL )
   {
      return;
   }

   // go through the nodes and add them to the graph
   std::set< RenderingPipelineNode* > analyzedNodes;
   std::list< RenderingPipelineNode* > nodesToAnalyze;
   nodesToAnalyze.push_back( startNode );

   while( !nodesToAnalyze.empty() )
   {
      RenderingPipelineNode* checkedNode = nodesToAnalyze.front();
      nodesToAnalyze.pop_front();

      // make sure the node's not already in the cache - we don't allow the same node to be updated twice
      // with this algorithm
      std::set< RenderingPipelineNode* >::const_iterator analyzedNodeIt = analyzedNodes.find( checkedNode );
      if ( analyzedNodeIt != analyzedNodes.end() )
      {
         continue;
      }

      RPGraph::Index nodeIdx = outGraph.getNodeIdx( checkedNode );

      std::vector< RenderingPipelineNode* > neighbors;
      checkedNode->getSubsequentNodes( neighbors );
      for ( std::vector< RenderingPipelineNode* >::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it )
      {
         RPGraph::Index neighboringNodeIdx = outGraph.getNodeIdx( *it );

         // create a connection in the graph
         outGraph.connect( nodeIdx, neighboringNodeIdx );

         // put he neighbor up for analysis
         nodesToAnalyze.push_back( *it );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

bool RenderingPipeline::addRenderTarget( RenderTargetDescriptor* descriptor )
{
   if ( descriptor == NULL )
   {
      ASSERT_MSG( descriptor != NULL, "NULL pointer specified instead a valid render target descriptor" );
      return false;
   }

   // check if the specified render target doesn't share an id with already registered one
   RenderTargetDescriptor* existingDescriptor = findRenderTarget( descriptor->getTargetID() );
   if ( existingDescriptor != NULL )
   {
      ASSERT_MSG( descriptor == NULL, "The specified descriptor has an ID used by another render target." );
      delete descriptor;
      return false;
   }

   m_renderTargets.push_back( descriptor );
   return true;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::removeRenderTarget( const std::string& id )
{
   unsigned int count = m_renderTargets.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_renderTargets[i]->getTargetID() == id )
      {
         notify( RPO_PRE_CHANGE );

         delete m_renderTargets[i];
         m_renderTargets.erase( m_renderTargets.begin() + i );

         notify( RPO_POST_CHANGE );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderTarget& RenderingPipeline::getRenderTarget( const std::string& id, RuntimeDataBuffer& runtimeData ) const
{
   RenderTargetDescriptor* desc = findRenderTarget( id );
   if ( desc == NULL )
   {
      char errorMsg[512];
      sprintf_s( errorMsg, "Render target '%s' not found", id.c_str() );
      ASSERT_MSG( desc != NULL, errorMsg );
      throw std::runtime_error( errorMsg );
   }
   
   return desc->getTarget( runtimeData );
}

///////////////////////////////////////////////////////////////////////////////

RenderTargetDescriptor* RenderingPipeline::findRenderTarget( const std::string& id ) const
{
   unsigned int count = m_renderTargets.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_renderTargets[i]->getTargetID() == id )
      {
         return m_renderTargets[i];
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

RenderTargetDescriptor& RenderingPipeline::lockRenderTarget( const std::string& id )
{
   if ( !m_lockedRT.empty() )
   {
      throw std::runtime_error( "Another render target is already locked" );
   }

   RenderTargetDescriptor* desc = findRenderTarget( id );
   if ( desc == NULL )
   {
      char errorMsg[512];
      sprintf_s( errorMsg, "Render target '%s' not found", id.c_str() );
      ASSERT_MSG( desc != NULL, errorMsg );
      throw std::runtime_error( errorMsg );
   }

   m_lockedRT = id;
   notify( RPO_PRE_CHANGE );
   return *desc;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::unlockRenderTarget( const std::string& id )
{
   if ( m_lockedRT.empty() || m_lockedRT != id )
   {
      throw std::runtime_error( "Trying to unlock a render target that hasn't been locked" );
   }
   m_lockedRT = "";
   notify( RPO_POST_CHANGE );
}

///////////////////////////////////////////////////////////////////////////////
