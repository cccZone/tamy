#ifndef _GRAPH_DIJKSTRA_H
#define _GRAPH_DIJKSTRA_H

/// @file   core/GraphDijkstra.h
/// @brief  dijkstra algorithm operating on tamy's graph representation


///////////////////////////////////////////////////////////////////////////////

/**
 * Performs the graph simplification using the Dijkstra algorithm.
 *
 * The function can only operate on graphs whose edges have 
 * a 'int getEdgeCost() const' method defined.
 *
 * @param outGraph            output graph that upon the function's return
 *                            will contain the simplified version of the input 
 *                            graph
 * @param inGraph             graph we want to run algorithm on
 * @param start               index of a node in the graph we want to start
 *                            searching from
 */
template<typename NODE, typename EDGE>
void GraphDijkstra(Graph<NODE, EDGE>& outGraph,
                   const Graph<NODE, EDGE>& inGraph, 
                   typename Graph<NODE, EDGE>::Index start);

///////////////////////////////////////////////////////////////////////////////

#include "core\GraphDijkstra.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_DIJKSTRA_H
