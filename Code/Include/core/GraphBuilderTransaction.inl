#ifndef _GRAPH_BUILDER_TRANSACTION_H
#error "This file can only be included from GraphBuilderTransaction.h"
#else

#include <set>


///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
GraphBuilderTransaction< Impl, NodeType >::GraphBuilderTransaction( GraphBuilder< Impl, NodeType >& graph )
   : m_graph( graph )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
GraphBuilderTransaction< Impl, NodeType >::~GraphBuilderTransaction()
{
   // clean remaining data
   uint count = m_connectionsToAdd.size();
   for ( uint i = 0; i < count; ++i )
   {
      delete m_connectionsToAdd[i];
   }
   m_connectionsToAdd.clear();

   count = m_connectionsToRemove.size();
   for ( uint i = 0; i < count; ++i )
   {
      delete m_connectionsToRemove[i];
   }
   m_connectionsToRemove.clear();
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::commit()
{
   // inform listeners that the graph's about to change
   m_graph.notifyPreChange();

   // apply changes
   std::set< NodeType* > changedOutputs;
   std::set< NodeType* > changedInputs;
   {
      // remove connections
      uint count = m_connectionsToRemove.size();
      for ( uint i = 0; i < count; ++i )
      {
         const ConnectionData* data = m_connectionsToRemove[i];
         if ( data->m_inputName.empty() )
         {
            // break all connections between nodes
            disconnect( data->m_startNode, data->m_endNode );
         }
         else
         {
            // break only the connection that ends in the specified input socket
            disconnect( data->m_endNode, data->m_inputName );
         }

         changedOutputs.insert( data->m_startNode );
         changedInputs.insert( data->m_endNode );
      }

      // add connections
      count = m_connectionsToAdd.size();
      for ( uint i = 0; i < count; ++i )
      {
         const ConnectionData* data = m_connectionsToAdd[i];
         connect( data->m_startNode, data->m_outputName, data->m_endNode, data->m_inputName );

         changedOutputs.insert( data->m_startNode );
         changedInputs.insert( data->m_endNode );
      }

      // remove obsolete nodes
      count = m_nodesToRemove.size();
      for ( uint i = 0; i < count; ++i )
      {
         NodeType* node = m_nodesToRemove[i];
         
         std::set< NodeType* >::const_iterator nodesIt = changedInputs.find( node );
         if ( nodesIt != changedInputs.end() )
         {
            changedInputs.erase( nodesIt );
         }

         nodesIt = changedOutputs.find( node );
         if ( nodesIt != changedOutputs.end() )
         {
            changedOutputs.erase( nodesIt );
         }

         m_graph.removeNode( *node );
      }

      // add new nodes
      count = m_nodesToAdd.size();
      for ( uint i = 0; i < count; ++i )
      {
         m_graph.addNode( m_nodesToAdd[i] );
      }
   }

   // commit implementation specific data
   onCommit( static_cast< Impl& >( m_graph ) );

   // inform listeners that the graph and nodes have changed
   {
      m_graph.notifyPostChange();

      for ( std::set< NodeType* >::const_iterator it = changedInputs.begin(); it != changedInputs.end(); ++it )
      {
         (*it)->notifyInputsChanged();
      }

      for ( std::set< NodeType* >::const_iterator it = changedOutputs.begin(); it != changedOutputs.end(); ++it )
      {
         (*it)->notifyOutputsChanged();
      }
   }

   // cleanup
   {
      m_nodesToRemove.clear();
      m_nodesToAdd.clear();

      uint count = m_connectionsToAdd.size();
      for ( uint i = 0; i < count; ++i )
      {
         delete m_connectionsToAdd[i];
      }
      m_connectionsToAdd.clear();

      count = m_connectionsToRemove.size();
      for ( uint i = 0; i < count; ++i )
      {
         delete m_connectionsToRemove[i];
      }
      m_connectionsToRemove.clear();
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::addNode( NodeType* node )
{
   putNodeForTransaction( node, m_nodesToAdd );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::removeNode( NodeType& node )
{
   putNodeForTransaction( &node, m_nodesToRemove );

   // disconnect the node from other nodes
   const std::vector< NodeType* >& nodes = m_graph.getNodes();
   uint nodesCount = nodes.size();
   for ( uint i = 0; i < nodesCount; ++i )
   {
      NodeType* otherNode = nodes[i];
      if ( !otherNode || otherNode == &node )
      {
         continue;
      }

      if ( node.isConnectedWith( otherNode ) )
      {
         disconnectNodes( &node, otherNode );
      }
      else if ( otherNode->isConnectedWith( &node ) )
      {
         disconnectNodes( otherNode, &node );
      }
   }

}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::putNodeForTransaction( NodeType* node, std::vector< NodeType* >& collection ) const
{
   // look for duplicates
   uint count = collection.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( collection[i] == node )
      {
         // this one's already on the list
         return;
      }
   }

   collection.push_back( node );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
bool GraphBuilderTransaction< Impl, NodeType >::connectNodes( NodeType* startNode, const std::string& outputName, NodeType* endNode, const std::string& inputName )
{
   if ( !startNode || !endNode )
   {
      return false;
   }

   TOutputSocket* output = startNode->findOutput( outputName );
   if ( !output )
   {
      return false;
   }


   // looking at a connection from input's perspective, a node is interested in an output 
   // it originates at - it's from where we're gonna get our data
   TInputSocket* input = endNode->findInput( inputName );
   if ( !input )
   {
      return false;
   }

   m_connectionsToAdd.push_back( new ConnectionData( startNode, outputName, endNode, inputName ) );
   return true;
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::disconnectNodes( NodeType* startNode, NodeType* endNode, const std::string& inputName )
{
   if ( !startNode || !endNode )
   {
      return;
   }

   m_connectionsToRemove.push_back( new ConnectionData( startNode, "", endNode, inputName ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::disconnectNodes( NodeType* startNode, NodeType* endNode )
{
   if ( !startNode || !endNode )
   {
      return;
   }

   m_connectionsToRemove.push_back( new ConnectionData( startNode, "", endNode, "" ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::connect( NodeType* startNode, const std::string& outputName, NodeType* destNode, const std::string& inputName )
{
   // looking at a connection from input's perspective, a node is interested in a node 
   // the connection connects this node to - it's a node that's gonna get 
   // activated after this node's update is finished
   TOutputSocket* output = startNode->findOutput( outputName );
   ASSERT_MSG( output, "Output socket not found" );
   output->connect( *destNode );

   // looking at a connection from input's perspective, a node is interested in an output 
   // it originates at - it's from where we're gonna get our data
   TInputSocket* input = destNode->findInput( inputName );
   ASSERT_MSG( input, "Input socket not found" );
   input->connect( *output );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::disconnect( NodeType* destNode, const std::string& inputName )
{
   // disconnect the other node's input
   TInputSocket* input = destNode->findInput( inputName );
   if ( !input )
   {
      // input could have been removed - and since there's nothing to do here, bail
      return;
   }

   // find the connected output
   TOutputSocket* output = input->getOutput();

   // disconnect the input
   input->disconnect();

   // disconnect the output
   if ( output )
   {
      output->disconnect( *destNode );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::disconnect( NodeType* startNode, NodeType* destNode )
{
   const std::vector< TOutputSocket* >& outputs = startNode->getOutputs();

   uint outputsCount = outputs.size();
   for ( uint i = 0; i < outputsCount; ++i )
   {
      TOutputSocket* output = outputs[i];
      if ( output )
      {
         output->disconnect( *destNode );
      }
   }

   // collect all connected inputs by filtering out the ones that are not connected
   // to startNode
   const std::vector< TInputSocket* >& inputs = destNode->getInputs();
   int inputsCount = inputs.size();
   for ( int i = inputsCount - 1; i >= 0; --i )
   {
      TInputSocket* input = inputs[i];

      for ( uint i = 0; i < outputsCount; ++i )
      {
         TOutputSocket* output = outputs[i];
         if ( output && input->getOutput() == output )
         {
            // found a connection - disconnect it
            input->disconnect();
            break;
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_TRANSACTION_H