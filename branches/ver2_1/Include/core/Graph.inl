#ifndef _GRAPH_H
#error "This file can only be included from Graph.h"
#else


///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
typename Graph<NODE, EDGE>::Index Graph<NODE, EDGE>::addNode(const NODE& node)
{
    Index idx = m_nodes.size();
    m_nodes.push_back(node);
    m_graphRepr.push_back(EdgeIndices());

    return idx;
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
unsigned int Graph<NODE, EDGE>::getNodesCount() const
{
   return m_nodes.size();
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
typename  Graph<NODE, EDGE>::Index Graph<NODE, EDGE>::connect(Index nodeIdx, const EDGE& edge)
{
    Index idx = m_edges.size();
    m_edges.push_back(edge);

    m_graphRepr[nodeIdx].push_back(idx);

    return idx;
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
unsigned int Graph<NODE, EDGE>::getEdgesCount() const
{
   return m_edges.size();
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
EDGE& Graph<NODE, EDGE>::getEdge(Index idx)
{
    return m_edges[idx];
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
const EDGE& Graph<NODE, EDGE>::getEdge(Index idx) const
{
    return m_edges[idx];
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
NODE& Graph<NODE, EDGE>::getNode(Index idx)
{
    return m_nodes[idx];
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
const NODE& Graph<NODE, EDGE>::getNode(Index idx) const
{
    return m_nodes[idx];
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
typename Graph<NODE, EDGE>::EdgeIndices& Graph<NODE, EDGE>::getEdges(Index nodeIdx) 
{
    return m_graphRepr[nodeIdx];
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
const typename Graph<NODE, EDGE>::EdgeIndices& Graph<NODE, EDGE>::getEdges(Index nodeIdx) const
{
    return m_graphRepr[nodeIdx];
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
NODE& Graph<NODE, EDGE>::getNode(const EDGE& edge)
{
    return getNode((Index)edge);
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
const NODE& Graph<NODE, EDGE>::getNode(const EDGE& edge) const
{
    return getNode((Index)edge);
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
typename Graph<NODE, EDGE>::Index Graph<NODE, EDGE>::getEdgeIdx(Index startNodeIdx, 
                                                                Index endNodeIdx) const
{
    const EdgeIndices& edges = m_graphRepr[startNodeIdx];
    for (EdgeIndices::const_iterator it = edges.begin();
        it != edges.end(); ++it)
    {
        if ((Index)(getEdge(*it)) == endNodeIdx)
        {
            return *it;
        }
    }

    return -1;
}

///////////////////////////////////////////////////////////////////////////////

template <typename NODE, typename EDGE>
void Graph<NODE, EDGE>::disconnect(Index nodeIdx)
{
    m_graphRepr[nodeIdx].clear();
}

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_H
