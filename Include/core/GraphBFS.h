#ifndef _GRAPH_BFS_H
#define _GRAPH_BFS_H

/// @file   core/GraphBFS.h
/// @brief  breadth-first search operating on tamy's graph representation

#include <list>


///////////////////////////////////////////////////////////////////////////////

/**
 * Performs a breadth-first search on the specified graph,
 * starting from the specified node.
 *
 * @param graph     graph we want to run BFS on
 * @param start     index of a node in the graph we want to start
 *                  searching from
 * @param operation     operation we want to perform on the nodes
 */
template<typename GRAPH, typename OPERATION>
void GraphBFS(const GRAPH& graph, 
                typename GRAPH::Index start, 
                OPERATION& operation);

///////////////////////////////////////////////////////////////////////////////

#include "core\GraphBFS.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BFS_H
