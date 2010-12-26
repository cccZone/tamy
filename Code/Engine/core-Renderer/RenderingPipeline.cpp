#include "core-Renderer/RenderingPipeline.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RPStartNode.h"
#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include <algorithm>
#include "core/Algorithms.h"


// TODO: jak juz bedzie wsjo dzialalo, zrobic ten resource const'owym - zrobic cos 
// jak runtime data buffer, gdzie node'y beda przechowywaly sobie dane na ktorych
// operuja, i niech tym buforem bedzie dla nich RenderingPipelineMechanism

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
   addObject( node );
   m_nodes.push_back( node );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::removeNode( RenderingPipelineNode& node )
{
   std::vector< RenderingPipelineNode* >::iterator it = std::find( m_nodes.begin(), m_nodes.end(), &node );
   if ( it != m_nodes.end() )
   {
      m_nodes.erase( it );
      removeObject( node.getObjectId() );
   }
   else
   {
      ASSERT_MSG( false, "Trying to remove a non-existent rendering pipeline node" );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::checkGraph()
{
   // TODO : !!!!!!! zrobic sprawdzanie poprawnosci grafu
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::initialize( RenderingPipelineMechanism& host )
{
   // initialize render targets
   for ( std::vector< RenderTargetDescriptor* >::iterator it = m_renderTargets.begin(); it != m_renderTargets.end(); ++it )
   {
      (*it)->initialize( host.getRenderer() );
   }

   // initialize nodes
   cacheNodes();
   for ( std::vector< RenderingPipelineNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
   {
      (*it)->initialize( host );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::deinitialize( RenderingPipelineMechanism& host )
{
   // deintialize nodes
   for ( std::vector< RenderingPipelineNode* >::iterator it = m_nodesQueue.begin(); it != m_nodesQueue.end(); ++it )
   {
      (*it)->deinitialize( host );
   }
   m_nodesQueue.clear();

   // deinitialize render targets
   for ( std::vector< RenderTargetDescriptor* >::iterator it = m_renderTargets.begin(); it != m_renderTargets.end(); ++it )
   {
      (*it)->deinitialize();
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::render( RenderingPipelineMechanism& host )
{
   unsigned int count = m_nodesQueue.size();
   for( unsigned int i = 0; i < count; ++i )
   {
      m_nodesQueue[i]->update( host );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::cacheNodes()
{
   m_nodesQueue.clear();

   // find the start node
   RenderingPipelineNode* startNode = NULL;
   for ( std::vector< RenderingPipelineNode* >::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it )
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

      const std::vector< RenderingPipelineNode* >& neighbors = checkedNode->getNeighbors();
      for ( std::vector< RenderingPipelineNode* >::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it )
      {
         nodesToCheck.push_back( *it );
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
         delete m_renderTargets[i];
         m_renderTargets.erase( m_renderTargets.begin() + i );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderTarget& RenderingPipeline::getRenderTarget( const std::string& id ) const
{
   RenderTargetDescriptor* desc = findRenderTarget( id );
   if ( desc == NULL )
   {
      char errorMsg[512];
      sprintf_s( errorMsg, "Render target '%s' not found", id.c_str() );
      ASSERT_MSG( desc != NULL, errorMsg );
      throw std::runtime_error( errorMsg );
   }
   
   return desc->getTarget();
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
