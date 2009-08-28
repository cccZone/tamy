#pragma once

/// @file   core/GraphBuilderFactory.h
/// @brief  interface required by the GraphBuider class

#include <string>
#include "core\Graph.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * GraphBuilder uses this class to create applicable nodes and edges.
 */
template<typename NODE, typename EDGE>
class GraphBuilderFactory
{
private:
   typedef Graph<NODE, EDGE> GRAPH;

public:
   virtual ~GraphBuilderFactory() {}

   /**
    * Creates a graph start node.
    *
    * @return     a graph node instance that can be used as a start node
    */
   virtual NODE createStartNode() = 0;

   /**
    * Creates a graph node. The name is specified so that if applicable,
    * the node can be given an ID.
    *
    * @param name    id for the node
    * @return        a graph node
    */
   virtual NODE createNode(const std::string& name) = 0;

   /**
    * Creates the graph edge.
    *
    * @param endNodeIdx    index of a node the edge points to
    * @return              a graph edge
    */
   virtual EDGE createEdge(typename GRAPH::Index endNodeIdx) = 0;
};

///////////////////////////////////////////////////////////////////////////////
