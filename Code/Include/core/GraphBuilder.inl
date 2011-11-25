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
void GraphBuilder< Impl, NodeType >::addNode( NodeType* node )
{
   notify( GBO_PRE_CHANGE );

   onNodeAdded( node );
   m_nodes.push_back( node );

   notify( GBO_POST_CHANGE );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilder< Impl, NodeType >::removeNode( NodeType& node )
{
   std::vector< NodeType* >::iterator it = std::find( m_nodes.begin(), m_nodes.end(), &node );
   if ( it != m_nodes.end() )
   {
      notify( GBO_PRE_CHANGE );

      m_nodes.erase( it );
      onNodeRemoved( node );

      notify( GBO_POST_CHANGE );
   }
   else
   {
      ASSERT_MSG( false, "Trying to remove a non-existing graph node" );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType > template< typename StartNodeType >
void GraphBuilder< Impl, NodeType >::buildGraph( Graph< NodeType* >& outGraph ) const
{
   // add all nodes to the graph, and find the start node while you're at it
   NodeType* startNode = NULL;
   for ( std::vector< NodeType* >::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it )
   {
      if ( startNode == NULL )
      {
         startNode = dynamic_cast< StartNodeType* >( *it );
      }

      outGraph.addNode( dynamic_cast< NodeType* >( *it ) );
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

      std::vector< NodeType* > neighbors;
      checkedNode->getSubsequentNodes( neighbors );
      for ( std::vector< NodeType* >::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it )
      {
         Graph< NodeType* >::Index neighboringNodeIdx = outGraph.getNodeIdx( *it );

         // create a connection in the graph
         outGraph.connect( nodeIdx, neighboringNodeIdx );

         // put he neighbor up for analysis
         nodesToAnalyze.push_back( *it );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////


#endif // _GRAPH_BUILDER_H