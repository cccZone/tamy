/// @file   core\Graph.h
/// @brief  graph representation
#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>
#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A default node that doesn't contain any additional information
 * except the direction of the connection (index of the end node).
 */
template < typename Node >
struct DefaultEdge;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a graph representation. It's a template that allows to use 
 * different node and edge types.
 * The only constraint is that an EDGE type had the Graph::Index operator
 * defined that returns an index of the node the edge points to.
 */
template <typename NODE, typename EDGE = DefaultEdge< NODE > >
class Graph
{
   DECLARE_ALLOCATOR( Graph, AM_ALIGNED_16 );

public:
    typedef int Index;
    enum {InvalidIndex = -1};

    typedef typename std::vector<Index> EdgeIndices;

private:
    std::vector<NODE>               m_nodes;  // a list of all nodes in the graph
    std::vector<EDGE>               m_edges;  // a list of all edges in the graph

    EdgeIndices                     m_edgesMapping;
    EdgeIndices                     m_freeEdgeMapings;

    // a square array - each col  represents a node index.
    // It's guaranteed to have as many columns as there are nodes in the system.
    // Each row in such a column contains an information about an edge that
    // exists from this node. An edge in return contains an information about
    // the node index it connects to at the other end - this way we can go 
    // from one node to another.
    std::vector< EdgeIndices >      m_graphRepr;

public:
    virtual ~Graph() {}

    /**
     * Adds a new node to the graph.
     *
     * @param node      new node
     * @return          index the node was assigned in the graph.
     */
    Index addNode( const NODE& node );

    /**
     * Returns index of the specified node.
     *
     * @param node
     */
    Index getNodeIdx( const NODE& node ) const;

    /**
     * The method returns the number of nodes the graph has
     *
     * @return          number of nodes in the graph
     */
    unsigned int getNodesCount() const;

    /**
     * Connects two nodes together. We need to specify the start
     * node explicitly, and the end node is specified implicitly,
     * confined in the EDGE class.
     *
     * @param nodeIdx   start node from which we want to start a connection
     * @param edge      edge that contains an information about the end node
     *                  and some other domain-specific stuff
     * @return          index the edge was assigned in the graph.
     */
    Index connect( Index nodeIdx, const EDGE& edge );

    /**
     * The method returns the number of edges the graph has
     *
     * @return          number of edges in the graph
     */
    unsigned int getEdgesCount() const;

    /**
     * Returns an edge stored in the graph under the specified index.
     *
     * @param idx       index of an edge we want to retrieve
     * @return          an edge
     */
    EDGE& getEdge( Index idx );

    /**
     * Returns an edge stored in the graph under the specified index.]
     * (const version)
     *
     * @param idx       index of an edge we want to retrieve
     * @return          an edge
     */
    const EDGE& getEdge( Index idx ) const;

    /**
     * Returns a node stored in the graph under the specified index.
     *
     * @param idx       index of a node we want to retrieve
     * @return          a node
     */
    NODE& getNode( Index idx );

    /**
     * Returns a node  stored in the graph under the specified index.]
     * (const version)
     *
     * @param idx       index of a node we want to retrieve
     * @return          a node
     */
    const NODE& getNode( Index idx ) const;

    /**
     * Returns all edges exiting the node that's registered in the graph
     * under the specified index.
     *
     * @param nodeIdx   index of a node
     * @return          vector containing all edges exiting the specified node
     */
    EdgeIndices& getEdges( Index nodeIdx );

    /**
     * Returns all edges exiting the node that's registered in the graph
     * under the specified index.
     *
     * @param nodeIdx   index of a node
     * @return          vector containing all edges exiting the specified node
     */
    const EdgeIndices& getEdges( Index nodeIdx ) const;

    /**
     * Returns all edges entering the node that's registered in the graph
     * under the specified index.
     *
     * @param nodeIdx   index of a node
     * @param outEdges  vector containing all edges entering the specified node
     */
    void getIncomingEdges( Index nodeIdx, typename Graph<NODE, EDGE>::EdgeIndices& outEdges ) const;

    /**
     * Returns a node the specified edge points to.
     *
     * @param edge      edge the end node of which we want to explore
     * @return          a node
     */
    NODE& getNode( const EDGE& edge );

    /**
     * Returns a node the specified edge points to.
     * (const version)
     *
     * @param edge      edge the end node of which we want to explore
     * @return          a node
     */
    const NODE& getNode( const EDGE& edge ) const;

    /**
     * Returns an index of an edge that connects two nodes
     * indices of which we pass as params.
     *
     * @param startNodeIdx  start node index
     * @param endNodeIdx    end node index
     * @return              index of an edge or -1, if there's no edge
     *                      connecting the two nodes
     */
    Index getEdgeIdx( Index startNodeIdx, Index endNodeIdx ) const;

    /**
     * Disconnects all edges exiting the specified node.
     *
     * @param nodeIdx       index of a node we want disconnected
     */
    void disconnect( Index nodeIdx );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A default node that doesn't contain any additional information
 * except the direction of the connection (index of the end node).
 */
template < typename Node >
struct DefaultEdge
{
   typedef Graph< Node, DefaultEdge< Node > > DefaultGraph;

   typename DefaultGraph::Index endNodeIdx;

   DefaultEdge( typename DefaultGraph::Index _endNodeIdx )
      : endNodeIdx( _endNodeIdx ) 
   {}

   operator typename DefaultGraph::Index() const { return endNodeIdx; }
};

///////////////////////////////////////////////////////////////////////////////

#include "core\Graph.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_H
