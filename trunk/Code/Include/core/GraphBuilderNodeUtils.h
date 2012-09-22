/// @file   core/GraphBuilderNodeUtils.h
/// @brief  a bunch of algorithms used by the graph builder node
#ifndef _GRAPH_BUILDER_NODE_UTILS_H
#define _GRAPH_BUILDER_NODE_UTILS_H


///////////////////////////////////////////////////////////////////////////////

/**
 * A bunch of algorithms used by the graph builder node.
 */
class GraphBuilderNodeUtils
{
public:
   /**
    * Merges two lists of sockets, deleting the obsolete ones from both of them.
    * The results will be stored in the `existingSockets` array.
    *
    * @param newSockets
    * @param existingSockets
    */
   template< typename SocketType >
   static int mergeSocketsLists( const std::vector< SocketType* >& newSockets, std::vector< SocketType* >& existingSockets );
};

///////////////////////////////////////////////////////////////////////////////

#include "core/GraphBuilderNodeUtils.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_NODE_UTILS_H
