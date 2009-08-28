#ifndef _GRAPH_BFS_H
#error "This file can only be included from GraphBFS.h"
#else


///////////////////////////////////////////////////////////////////////////////

template <typename GRAPH, typename OPERATION>
void GraphBFS(const GRAPH& graph, 
              typename GRAPH::Index start, 
              OPERATION& operation)
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

#endif // _GRAPH_BFS_H
