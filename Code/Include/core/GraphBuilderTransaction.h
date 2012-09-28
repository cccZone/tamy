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
   typedef GBNodeInput< NodeType > TInputSocket;
   typedef GBNodeOutput< NodeType > TOutputSocket;

   struct ConnectionData
   {
       NodeType*        m_startNode;
       std::string      m_outputName;
       NodeType*        m_endNode;
       std::string      m_inputName;

       ConnectionData( NodeType* startNode, const std::string& outputName, NodeType* endNode, const std::string& inputName )
          : m_startNode( startNode )
          , m_outputName( outputName )
          , m_endNode( endNode )
          , m_inputName( inputName )
       {
       }
   };

protected:
   GraphBuilder< Impl, NodeType >&     m_graph;

   std::vector< NodeType* >            m_nodesToAdd;
   std::vector< NodeType* >            m_nodesToRemove;

   std::vector< ConnectionData* >      m_connectionsToAdd;
   std::vector< ConnectionData* >      m_connectionsToRemove;

public:
   /**
    * Constructor.
    *
    * @param graph      changed graph
    */
   GraphBuilderTransaction( GraphBuilder< Impl, NodeType >& graph );
   virtual ~GraphBuilderTransaction();

   /**
    * Commits a transaction.
    *
    * @param graph
    */
   void commit();

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
    * Connects two nodes with a one-directional connection that starts in starttNode's socket named 'outputName'
    * and ends in endNode's socket names 'inputName'.
    *
    * @param startNode
    * @param outputName
    * @param endNode
    * @param inputName
    *
    * @return  'true' if the connection was successfully established
    */
   bool connectNodes( NodeType* startNode, const std::string& outputName, NodeType* endNode, const std::string& inputName );

   /**
    * Disconnects two nodes, breaking the connection that connects startNode with endNode's 'inputName' socket.
    *
    * @param startNode
    * @param endNode
    * @param inputName
    */
   void disconnectNodes( NodeType* startNode, NodeType* endNode, const std::string& inputName );

   /**
    * Disconnects two nodes, breaking all existing connections between them.
    * @param startNode
    * @param endNode
    */
   void disconnectNodes( NodeType* startNode, NodeType* endNode );

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

private:
   /**
    * Establishes a connection between this node's output `outputName`,
    * and the `destNode's` input `inputName`
    *
    * @param outputName
    * @param destNode
    * @param inputName
    */
   void connect( NodeType* startNode, const std::string& outputName, NodeType* destNode, const std::string& inputName );

   /**
    * Disconnects this node from the specified node's input.
    *
    * @param destNode
    * @param inputName
    */
   void disconnect( NodeType* destNode, const std::string& inputName );

   /**
    * Disconnects this node from the specified node - disconnects all involved inputs.
    *
    * @param destNode
    */
   void disconnect( NodeType* startNode, NodeType* destNode );
};

///////////////////////////////////////////////////////////////////////////////

#include "core/GraphbuilderTransaction.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_TRANSACTION_H
