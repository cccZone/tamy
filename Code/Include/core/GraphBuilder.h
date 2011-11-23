/// @file   core/GraphBuilder.h
/// @brief  graph builder resource
#ifndef _GRAPH_BUILDER_H
#define _GRAPH_BUILDER_H

#include "core/Resource.h"
#include <vector>
#include "core/Subject.h"
#include "core/Graph.h"


///////////////////////////////////////////////////////////////////////////////

class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

enum GraphBuilderOperation
{
   GBO_PRE_CHANGE,
   GBO_POST_CHANGE
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Graph builder resource.
 */
template< typename Impl, typename NodeType >
class GraphBuilder : public Subject< Impl, GraphBuilderOperation >
{
protected:
   std::vector< NodeType* >        m_nodes;

public:
   /**
    * Constructor.
    */
   GraphBuilder();
   virtual ~GraphBuilder();

   // -------------------------------------------------------------------------
   // Nodes management
   // -------------------------------------------------------------------------
   /**
    * Adds a new node to the graph.
    *
    * @param node       node to add
    */
   void addNode( NodeType* node );

   /**
    * Removes a node from the graph
    *
    * @param node       node to remove
    */
   void removeNode( NodeType& node );

   /**
    * Returns a list of all defined nodes.
    */
   inline const std::vector< NodeType* >& getNodes() const { return m_nodes; }

   /**
    * Constructs a graph based on the definition.
    *
    * @param outGraph      resulting graph
    * @param startNodeType
    */
   template< typename StartNodeType >
   void buildGraph( Graph< NodeType* >& outGraph ) const;

protected:
   /**
    * Informs the implementation that a node's just been added.
    *
    * @param node
    */
   virtual void onNodeAdded( NodeType* node ) = 0;

   /**
    * Informs the implementation that a node's about to be removed.
    *
    * @param node
    */
   virtual void onNodeRemoved( NodeType& node ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

#include "core/GraphBuilder.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_H
