#ifndef _GRAPH_H
#error "This file can only be included from Graph.h"
#else

#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
typename Graph<NODE, EDGE>::Index Graph<NODE, EDGE>::addNode( const NODE& node )
{
   Index idx = m_nodes.size();
   m_nodes.push_back(node);
   m_graphRepr.push_back(EdgeIndices());

   return idx;
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
typename Graph< NODE, EDGE >::Index Graph< NODE, EDGE >::getNodeIdx( const NODE& node ) const
{
   for (  std::vector<NODE>::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it )
   {
      if ( *it == node )
      {
         return it - m_nodes.begin();
      }
   }

   return InvalidIndex;
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
unsigned int Graph<NODE, EDGE>::getNodesCount() const
{
   return m_nodes.size();
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
typename  Graph<NODE, EDGE>::Index Graph<NODE, EDGE>::connect( Index nodeIdx, const EDGE& edge )
{
   Index idx = m_edges.size();
   m_edges.push_back(edge);

   Index edgeIdx = InvalidIndex;
   if ( m_freeEdgeMapings.empty() )
   {
      edgeIdx = m_edgesMapping.size();
      m_edgesMapping.push_back( idx );
   }
   else
   {
      edgeIdx = m_freeEdgeMapings.back();
      m_freeEdgeMapings.pop_back();
      m_edgesMapping[ edgeIdx ] = idx;
   }

   m_graphRepr[nodeIdx].push_back( edgeIdx );

   return idx;
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
unsigned int Graph<NODE, EDGE>::getEdgesCount() const
{
   return m_edgesMapping.size() - m_freeEdgeMapings.size();
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
EDGE& Graph<NODE, EDGE>::getEdge( Index idx )
{
   if ( m_edgesMapping[idx] != InvalidIndex )
   {
      return m_edges[ m_edgesMapping[idx] ];
   }
   else
   {
      ASSERT_MSG( false, "Trying to access a non-existing edge" );
      return *((EDGE*)NULL);
   }
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
const EDGE& Graph<NODE, EDGE>::getEdge( Index idx ) const
{
   if ( m_edgesMapping[idx] != InvalidIndex )
   {
      return m_edges[ m_edgesMapping[idx] ];
   }
   else
   {
      ASSERT_MSG( false, "Trying to access a non-existing edge" );
      return *((EDGE*)NULL);
   }
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
NODE& Graph<NODE, EDGE>::getNode( Index idx )
{
   return m_nodes[idx];
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
const NODE& Graph<NODE, EDGE>::getNode( Index idx ) const
{
   return m_nodes[idx];
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
typename Graph<NODE, EDGE>::EdgeIndices& Graph<NODE, EDGE>::getEdges( Index nodeIdx ) 
{
   return m_graphRepr[nodeIdx];
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
const typename Graph<NODE, EDGE>::EdgeIndices& Graph<NODE, EDGE>::getEdges( Index nodeIdx ) const
{
   return m_graphRepr[nodeIdx];
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
void Graph< NODE, EDGE >::getIncomingEdges( Index nodeIdx, typename Graph< NODE, EDGE >::EdgeIndices& outEdges ) const
{
   EdgeIndices::const_iterator startIt = m_edgesMapping.begin();
   for ( EdgeIndices::const_iterator it = startIt; it != m_edgesMapping.end(); ++it )
   {
      if ( *it != InvalidIndex && ( Index )( m_edges[ *it ] ) == nodeIdx )
      {
         outEdges.push_back( it - startIt );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
NODE& Graph<NODE, EDGE>::getNode( const EDGE& edge )
{
   return getNode( (Index)edge );
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
const NODE& Graph<NODE, EDGE>::getNode( const EDGE& edge ) const
{
   return getNode( (Index)edge );
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
typename Graph<NODE, EDGE>::Index Graph<NODE, EDGE>::getEdgeIdx( Index startNodeIdx, 
                                                                 Index endNodeIdx ) const
{
   const EdgeIndices& edges = m_graphRepr[startNodeIdx];
   for ( EdgeIndices::const_iterator it = edges.begin(); it != edges.end(); ++it )
   {
       if ( ( Index )( getEdge( *it ) ) == endNodeIdx )
       {
           return *it;
       }
   }

   return -1;
}

///////////////////////////////////////////////////////////////////////////////

template < typename NODE, typename EDGE >
void Graph< NODE, EDGE >::disconnect( Index nodeIdx )
{
   for ( EdgeIndices::iterator it = m_graphRepr[nodeIdx].begin(); it != m_graphRepr[nodeIdx].end(); ++it )
   {
      m_edgesMapping[ *it ] = InvalidIndex;
      m_freeEdgeMapings.push_back( *it );
   }
   m_graphRepr[nodeIdx].clear();
}

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_H
