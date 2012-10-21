/// @file   core/GraphBuilder.h
/// @brief  graph builder resource
#ifndef _GRAPH_BUILDER_H
#define _GRAPH_BUILDER_H

#include "core\Resource.h"
#include <vector>
#include "core\Subject.h"
#include "core\Graph.h"


///////////////////////////////////////////////////////////////////////////////

class RuntimeDataBuffer;

template< typename Impl, typename NodeType >
class GraphBuilderTransaction;

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
public:
   std::vector< NodeType* >        m_nodes;

public:
   /**
    * Constructor.
    */
   GraphBuilder();
   virtual ~GraphBuilder();

   // -------------------------------------------------------------------------
   // Graph instancing
   // -------------------------------------------------------------------------
   /**
    * Returns a list of all defined nodes.
    */
   inline const std::vector< NodeType* >& getNodes() const { return m_nodes; }

   /**
    * Constructs a graph based on the definition.
    *
    * Edges start in outputs and end in inputs. The graph is built from one designated start node
    * towards multiple end nodes.
    *
    * @param outGraph      resulting graph
    * @param StartNodeType
    */
   template< typename StartNodeType >
   void buildGraph( Graph< NodeType* >& outGraph ) const;

   /**
    * Constructs a graph based on the definition.
    *
    * Edges start in inputs and end in outputs. This means that the graph is built backwards -
    * from the end node to the start nodes. This means that there may be many start nodes, but only one end node.
    *
    *
    * @param outGraph      resulting graph
    * @param EndNodeType
    */
   template< typename EndNodeType >
   void buildReverseGraph( Graph< NodeType* >& outGraph ) const;

   /**
    * Analyzes the specified nodes domain in search of the nodes
    * that precede the checkedNode in the graph ( whose outputs connect to that node's inputs ).
    *
    * @param checkedNode
    * @param inAllNodes       nodes we want to check
    * @param outPredecessors
    */
   void getPredecessors( NodeType* checkedNode, std::vector< NodeType* >& outPredecessors ) const;

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

   
private:
   // -------------------------------------------------------------------------
   // Transaction API
   // -------------------------------------------------------------------------
   friend class GraphBuilderTransaction< Impl, NodeType >;

   /**
    * Transaction code calls this method before it changes the graph state.
    */
   void notifyPreChange();

   /**
    * Transaction code calls this method after it changes the graph state.
    */
   void notifyPostChange();

   /**
    * Transaction adds a new node to the graph using this method.
    *
    * @param node       node to add
    */
   void addNode( NodeType* node );

   /**
    * Transaction removes a node from the graph using this method.
    *
    * @param node       node to remove
    */
   void removeNode( NodeType& node );

};

///////////////////////////////////////////////////////////////////////////////

#include "core/GraphBuilder.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_H
