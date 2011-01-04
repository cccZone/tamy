#ifndef _GRAPH_ALGORITHMS_H
#error "This file can only be included from GraphAlgorithms.h"
#else

#include <algorithm>
#include <vector>
#include <list>
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   template<typename GRAPH>
   class NodesComparator
   {
   private:
      std::vector<int>& m_nodesCosts;

   public:
      NodesComparator(std::vector<int>& nodesCosts)
         : m_nodesCosts(nodesCosts)
      {}

      /**
       * We need to sort in the descending order - algorithm requirements.
       */
      bool operator()(typename GRAPH::Index rhs, typename GRAPH::Index lhs) const
      {
         return m_nodesCosts[rhs] > m_nodesCosts[lhs];
      }
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

template < typename GRAPH, typename OPERATION >
void GraphBFS( const GRAPH& graph, 
               typename GRAPH::Index start, 
               OPERATION& operation )
{
   std::list<GRAPH::Index> nodesQueue;
   nodesQueue.push_back(start);

   while (nodesQueue.size())
   {
      GRAPH::Index currNodeIdx = nodesQueue.front();
      nodesQueue.pop_front();

      operation(graph.getNode(currNodeIdx));

      const GRAPH::EdgeIndices& edges = graph.getEdges(currNodeIdx);
      for (GRAPH::EdgeIndices::const_iterator it = edges.begin();
         it != edges.end(); ++it)
      {
         nodesQueue.push_back((GRAPH::Index)graph.getEdge(*it));
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename NODE, typename EDGE >
void GraphDijkstra( Graph<NODE, EDGE>& outGraph,
                    const Graph<NODE, EDGE>& graph, 
                    typename Graph<NODE, EDGE>::Index start )
{
   if (outGraph.getNodesCount() > 0)
   {
      throw std::invalid_argument("Output graph needs to be passed empty");
   }

   // -------------------------------------------------------------------------
   // helper types
   // -------------------------------------------------------------------------
   typedef Graph< NODE, EDGE > GRAPH;
   typedef typename GRAPH::Index NodeIndex;
   typedef typename GRAPH::Index EdgeIndex;
   typedef typename GRAPH::EdgeIndices EdgesList;
   typedef std::vector< NodeIndex > NodesList;

   // -------------------------------------------------------------------------
   // algorithm
   // -------------------------------------------------------------------------

   std::vector<int> nodesCosts(graph.getNodesCount(), 0xFFFF);
   NodesList nodesParents(graph.getNodesCount(), typename GRAPH::InvalidIndex);
   NodesComparator<GRAPH> comparator(nodesCosts);

   NodesList m_closedList;
   NodesList m_openList;

   m_openList.push_back(start);
   nodesCosts[start] = 0;

   while (m_openList.size())
   {
      // find the cheapest node in the open nodes list and 'promote' it
      // to the closed list
      std::sort(m_openList.begin(), m_openList.end(), comparator);
      NodeIndex currNodeIdx = m_openList.back();
      m_openList.pop_back();

      m_closedList.push_back(currNodeIdx);

      // analyze the cheapest node's neighbors
      const EdgesList& adjacentEdges = graph.getEdges(currNodeIdx);
      for (EdgesList::const_iterator it = adjacentEdges.begin();
           it != adjacentEdges.end(); ++it)
      {
         EdgeIndex edgeIdx = *it;
         const EDGE& edge = graph.getEdge(edgeIdx);
         typename NodeIndex endNodeIdx = (NodeIndex)edge;

         // if the node's already in the closed list - discard it
         if (std::find(m_closedList.begin(), m_closedList.end(), endNodeIdx) != m_closedList.end())
         {
            continue;
         }

         int newCost = nodesCosts[currNodeIdx] + edge.getEdgeCost();

         // if the node's already in the open list, relax it, 
         // and if it's not - add it
         NodesList::iterator openNodeIt = std::find(m_openList.begin(), m_openList.end(), endNodeIdx);
         if (openNodeIt != m_openList.end())
         {
            if (nodesCosts[*openNodeIt] > newCost) 
            {
               nodesCosts[*openNodeIt] = newCost;
               nodesParents[*openNodeIt] = currNodeIdx;
            }
         }
         else
         {
            m_openList.push_back(endNodeIdx);
            nodesCosts[endNodeIdx] = newCost;
            nodesParents[endNodeIdx] = currNodeIdx;
         }
      }
   }

   // -------------------------------------------------------------------------
   // output graph formulation
   // -------------------------------------------------------------------------
   NodesList indexRemap(graph.getNodesCount(), typename GRAPH::InvalidIndex);
   NodeIndex newNodeIndex = 0;
   for (NodesList::iterator it = m_closedList.begin(); 
        it != m_closedList.end(); ++it, ++newNodeIndex)
   {
      outGraph.addNode(graph.getNode(*it));
      indexRemap[*it] = newNodeIndex;
   }
   
   unsigned int nodesCount = m_closedList.size();
   for (unsigned int newEndNodeIdx = 0; newEndNodeIdx < nodesCount; ++newEndNodeIdx)
   {
      NodeIndex oldEndNodeIdx = m_closedList[newEndNodeIdx];
      NodeIndex oldStartNodeIdx = nodesParents[oldEndNodeIdx];
      if (oldStartNodeIdx == typename GRAPH::InvalidIndex) {continue;}

      EdgeIndex oldEdgeIdx = graph.getEdgeIdx(oldStartNodeIdx, oldEndNodeIdx);
      EDGE newEdge = graph.getEdge(oldEdgeIdx);
      newEdge.endNodeIdx = newEndNodeIdx;

      NodeIndex newStartNodeIdx = indexRemap[oldStartNodeIdx];
      outGraph.connect(newStartNodeIdx, newEdge);
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename NODE, typename EDGE >
void GraphTopologicalSort( std::vector< typename Graph<NODE, EDGE>::Index >& outNodesArr,
                           const Graph< NODE, EDGE >& inGraph )
{
   // -------------------------------------------------------------------------
   // helper types
   // -------------------------------------------------------------------------
   typedef Graph< NODE, EDGE > GRAPH;
   typedef typename GRAPH::Index NodeIndex;
   typedef typename GRAPH::EdgeIndices EdgesList;
   typedef std::vector< NodeIndex > NodesList;

   // -------------------------------------------------------------------------
   // algorithm
   // -------------------------------------------------------------------------
   GRAPH tempGraph = inGraph;

   // find nodes that don't have any incoming connections, and add them as start nodes 
   std::list< NodeIndex >  nodesToAnalyze;
   NodeIndex nodesCount = (NodeIndex)tempGraph.getNodesCount();
   for ( NodeIndex i = 0; i < nodesCount; ++i )
   {
      EdgesList incomingEdgeIndices;
      tempGraph.getIncomingEdges( i, incomingEdgeIndices );
      if ( incomingEdgeIndices.empty() )
      {
         nodesToAnalyze.push_back( i );
      }
   }

   while( !nodesToAnalyze.empty() )
   {
      // remove the node from the temporary list of nodes
      NodeIndex nodeIdx = nodesToAnalyze.front();
      nodesToAnalyze.pop_front();

      // if the node's already in the final list - skip it
      NodesList::const_iterator finalListNodeIt = std::find( outNodesArr.begin(), outNodesArr.end(), nodeIdx );
      if ( finalListNodeIt != outNodesArr.end() )
      {
         continue;
      }
      // add the node to the final list of nodes
      outNodesArr.push_back( nodeIdx );

      // gather subsequent nodes
      const EdgesList& outEdgeIndices = tempGraph.getEdges( nodeIdx );
      NodesList nextNodes;
      for ( EdgesList::const_iterator it = outEdgeIndices.begin(); it != outEdgeIndices.end(); ++it )
      {
         NodeIndex idx = ( NodeIndex )( tempGraph.getEdge( *it ) );
         nextNodes.push_back( idx );
      }

      // remove the edges from the graph
      tempGraph.disconnect( nodeIdx );

      // analyze the subsequent nodes - and if they don't have any incoming edges,
      // add them to the temporary list
      for ( NodesList::const_iterator it = nextNodes.begin(); it != nextNodes.end(); ++it )
      {
         EdgesList incomingEdgeIndices;
         tempGraph.getIncomingEdges( *it, incomingEdgeIndices );
         if ( incomingEdgeIndices.empty() )
         {
            nodesToAnalyze.push_back( *it );
         }
      }
   }

   if ( tempGraph.getEdgesCount() > 0 )
   {
      throw std::runtime_error( "The graph is not a DAG" );
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_ALGORITHMS_H
