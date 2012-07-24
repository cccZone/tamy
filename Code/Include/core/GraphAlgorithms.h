/// @file   core/GraphAlgorithms.h
/// @brief  algorithms operating on graphs

#ifndef _GRAPH_ALGORITHMS_H
#define _GRAPH_ALGORITHMS_H


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
template< typename GRAPH, typename OPERATION >
void GraphBFS( const GRAPH& graph, 
               typename GRAPH::Index start, 
               OPERATION& operation );

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
template< typename NODE, typename EDGE >
void GraphDijkstra( Graph<NODE, EDGE>& outGraph,
                    const Graph<NODE, EDGE>& inGraph, 
                    typename Graph<NODE, EDGE>::Index start );

///////////////////////////////////////////////////////////////////////////////

/**
 * Sorts the graph's nodes in their topological order ( preserving the dependencies ).
 *
 * @param outNodesArr         output array that upon the function's return
 *                            will contain the sorted list of the original graph 
 *                            nodes
 * @param inGraph             graph we want to run algorithm on
 */
template< typename NODE, typename EDGE >
void GraphTopologicalSort( std::vector< typename Graph<NODE, EDGE>::Index >& outNodesArr, const Graph<NODE, EDGE>& inGraph );

///////////////////////////////////////////////////////////////////////////////

#include "core\GraphAlgorithms.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_ALGORITHMS_H
