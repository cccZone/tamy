#ifndef _GRAPH_BUILDER_H
#error "This file can only be included from GraphBuilder.h"
#else

#include "core/GraphBuilderNode.h"
#include <algorithm>
#include "core/Algorithms.h"


///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
GraphBuilder< Impl, NodeType >::GraphBuilder()
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
GraphBuilder< Impl, NodeType >::~GraphBuilder()
{
   m_nodes.clear();
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilder< Impl, NodeType >::notifyPreChange()
{
   notify( GBO_PRE_CHANGE );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilder< Impl, NodeType >::notifyPostChange()
{
   notify( GBO_POST_CHANGE );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilder< Impl, NodeType >::addNode( NodeType* node )
{
   onNodeAdded( node );
   m_nodes.push_back( node );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilder< Impl, NodeType >::removeNode( NodeType& node )
{
   std::vector< NodeType* >::iterator it = std::find( m_nodes.begin(), m_nodes.end(), &node );
   if ( it == m_nodes.end() )
   {
      ASSERT_MSG( false, "Trying to remove a non-existing graph node" );
      return;
   }
   
   // remove the node from the graph
   NodeType* removedNode = *it;
   m_nodes.erase( it );

   // disconnect the node from other nodes
   uint nodesCount = m_nodes.size();
   for ( uint i = 0; i < nodesCount; ++i )
   {
      if ( m_nodes[i] )
      {
         m_nodes[i]->disconnect( *removedNode );
      }
   }

   // send notifications
   onNodeRemoved( node );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType > template< typename StartNodeType >
void GraphBuilder< Impl, NodeType >::buildGraph( Graph< NodeType* >& outGraph ) const
{
   // add all nodes to the graph, and find the start node while you're at it
   NodeType* startNode = NULL;
   for ( std::vector< NodeType* >::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it )
   {
      NodeType* node = *it;
      if ( !node )
      {
         continue;
      }
      if ( startNode == NULL )
      {
         startNode = dynamic_cast< StartNodeType* >( *it );
      }

      outGraph.addNode( *it );
   }
   if ( startNode == NULL )
   {
      return;
   }

   // go through the nodes and add them to the graph
   std::set< NodeType* > analyzedNodes;
   std::list< NodeType* > nodesToAnalyze;
   nodesToAnalyze.push_back( startNode );

   while( !nodesToAnalyze.empty() )
   {
      NodeType* checkedNode = nodesToAnalyze.front();
      nodesToAnalyze.pop_front();

      // make sure the node's not already in the cache - we don't allow the same node to be updated twice
      // with this algorithm
      std::set< NodeType* >::const_iterator analyzedNodeIt = analyzedNodes.find( checkedNode );
      if ( analyzedNodeIt != analyzedNodes.end() )
      {
         continue;
      }

      Graph< NodeType* >::Index nodeIdx = outGraph.getNodeIdx( checkedNode );

      std::vector< NodeType* > successors;
      checkedNode->getSubsequentNodes( successors );
      for ( std::vector< NodeType* >::const_iterator it = successors.begin(); it != successors.end(); ++it )
      {
         NodeType* successorNode = *it;
         if ( !successorNode )
         {
            continue;
         }
         Graph< NodeType* >::Index successorNodeIdx = outGraph.getNodeIdx( successorNode );

         // create a connection in the graph
         outGraph.connect( nodeIdx, successorNodeIdx );

         // put the successor up for analysis
         nodesToAnalyze.push_back( successorNode );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType > template< typename EndNodeType >
void GraphBuilder< Impl, NodeType >::buildReverseGraph( Graph< NodeType* >& outGraph ) const
{
   // add all nodes to the graph, and find the end node while you're at it
   NodeType* endNode = NULL;
   for ( std::vector< NodeType* >::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it )
   {
      NodeType* node = *it;
      if ( !node )
      {
         continue;
      }
      if ( endNode == NULL )
      {
         endNode = dynamic_cast< EndNodeType* >( *it );
      }

      outGraph.addNode( *it );
   }
   if ( endNode == NULL )
   {
      return;
   }

   // go through the nodes and add them to the graph
   std::set< NodeType* > analyzedNodes;
   std::list< NodeType* > nodesToAnalyze;
   nodesToAnalyze.push_back( endNode );

   while( !nodesToAnalyze.empty() )
   {
      NodeType* checkedNode = nodesToAnalyze.front();
      nodesToAnalyze.pop_front();

      // make sure the node's not already in the cache - we don't allow the same node to be updated twice
      // with this algorithm
      std::set< NodeType* >::const_iterator analyzedNodeIt = analyzedNodes.find( checkedNode );
      if ( analyzedNodeIt != analyzedNodes.end() )
      {
         continue;
      }

      Graph< NodeType* >::Index nodeIdx = outGraph.getNodeIdx( checkedNode );

      std::vector< NodeType* > predecessors;
      getPredecessors( checkedNode, predecessors );
      for ( std::vector< NodeType* >::const_iterator it = predecessors.begin(); it != predecessors.end(); ++it )
      {
         NodeType* preceedingNode = *it;
         if ( !preceedingNode )
         {
            continue;
         }
         Graph< NodeType* >::Index preceedingNodeIdx = outGraph.getNodeIdx( preceedingNode );

         // create a connection in the graph
         outGraph.connect( nodeIdx, preceedingNodeIdx );

         // put the predecessor up for analysis
         nodesToAnalyze.push_back( preceedingNode );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilder< Impl, NodeType >::getPredecessors( NodeType* checkedNode, std::vector< NodeType* >& outPredecessors ) const
{
   uint count = m_nodes.size();
   for ( uint i = 0; i < count; ++i )
   {
      NodeType* node = m_nodes[i];
      if ( node->isConnectedWith( checkedNode ) )
      {
         outPredecessors.push_back( node );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_H