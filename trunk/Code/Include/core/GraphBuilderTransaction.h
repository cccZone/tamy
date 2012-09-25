/// @file   core/GraphBuilderTransaction.h
/// @brief  a single transaction that changes the state of a graph
#ifndef _GRAPH_BUILDER_TRANSACTION_H
#define _GRAPH_BUILDER_TRANSACTION_H

#include "core/GraphBuilder.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * A single transaction that changes the state of a graph.
 *
 * Introduced for the sole purpose of observer update messaging reduction, when
 * large amounts of data are changed about a graph.
 */
template< typename Impl, typename NodeType >
class GraphBuilderTransaction
{
protected:
   std::vector< NodeType* >         m_nodesToAdd;
   std::vector< NodeType* >         m_nodesToRemove;

public:
   /**
    * Constructor.
    *
    * @param graph      changed graph
    */
   GraphBuilderTransaction();
   virtual ~GraphBuilderTransaction();

   /**
    * Commits a transaction.
    *
    * @param graph
    */
   void commit( GraphBuilder< Impl, NodeType >& graph );

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

protected:
   /**
    * A helper method for managing a transaction collections.
    *
    * @param node
    * @param collection
    */
   void putNodeForTransaction( NodeType* node, std::vector< NodeType* >& collection ) const;

   /**
    * Perform an implementation-specific data commitment here
    *
    * @param graph
    */
   virtual void onCommit( Impl& graph ) {}
};

///////////////////////////////////////////////////////////////////////////////

#include "core/GraphbuilderTransaction.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_TRANSACTION_H
