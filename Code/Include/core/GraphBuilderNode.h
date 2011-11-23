/// @file   core/GraphBuilderNode.h
/// @brief  basic graph builder node
#ifndef _GRAPH_BUILDER_NODE_H
#define _GRAPH_BUILDER_NODE_H

#include "core/Resource.h"
#include "core/Subject.h"
#include "core/Observer.h"


///////////////////////////////////////////////////////////////////////////////

class GBNodeSocket;
enum GBNodeSocketOperation;
class GraphInstanceOwner;

///////////////////////////////////////////////////////////////////////////////

enum GraphBuilderNodeOperation
{
   GBNO_CHANGED,
   GBNO_INPUTS_CHANGED,
   GBNO_OUTPUTS_CHANGED,
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A graph builder node.
 *
 * A node keeps track of the nodes it's connected to ( for speed access
 * reasons ).
 *
 * There are two major socket types: input and output, and they follow these rules:
 * 1.) an output socket may have an infinite number of outgoing connections
 * 2.) an input socket may have only one incoming connection
 * 3.) an output socket has a type and is a data sink for the host node.
 *     If a node wants to store it's processing results for other nodes to
 *     use - an output socket is a place for that
 * 4.) an input socket is a data proxy - it takes data from an output socket
 *     it's connected to. Thus a strong typing is required here.
 * 5.) After an implementation-specific update takes place, the node
 *     schedules all outgoing connections for further processing with the host mechanism.
 */
template< typename Impl, typename TInputSocket, typename TOutputSocket >
class GraphBuilderNode : public Subject< Impl, GraphBuilderNodeOperation >, public Observer< GBNodeSocket, GBNodeSocketOperation >
{
protected:
   typedef std::vector< TInputSocket* >         InputsMap;
   typedef std::vector< TOutputSocket* >        OutputsMap;

protected:
   InputsMap                                    m_inputs;
   OutputsMap                                   m_outputs;

public:
   virtual ~GraphBuilderNode();

   /**
    * Returns nodes that should be run after this node.
    *
    * @param outNodesToRun    an array to append the nodes to
    */
   void getSubsequentNodes( std::vector<Impl* >& outNodesToRun ) const;

   // -------------------------------------------------------------------------
   // Connections management
   // -------------------------------------------------------------------------
   /**
    * Connects another node to this node's input.
    *
    * @param output           output we're connecting this input with
    * @param inputName
    *
    * @return                 'true' if the connection was established successfully, 'false' otherwise
    */
   bool connectToInput( TOutputSocket& output, const std::string& inputName );

   /**
    * Disconnects the specified input.
    *
    * @param inputName
    */
   void disconnectFromInput( const std::string& inputName );

   /**
    * Connects another node to this node's output.
    *
    * @param node             connected node
    * @param outputName
    */
   void connectToOutput( Impl& node, const std::string& outputName );

   /**
    * Disconnects a node from the specified output.
    *
    * @param node             connected node
    * @param outputName
    */
   void disconnectFromOutput( Impl& node, const std::string& outputName );
   
   /**
    * Returns an array of all defined input sockets.
    */
   inline const std::vector< TInputSocket* >& getInputs() const { return m_inputs; }

   /**
    * Returns an array of all defined output sockets.
    */
   inline const std::vector< TOutputSocket* >& getOutputs() const { return m_outputs; }

   /**
    * Looks for the specified input, or returns NULL if one can't be found.
    *
    * NOTE: even though the method returns a pointer, the caller is not supposed to destroy
    *       the returned object!
    *
    * @param inputName
    */
   TInputSocket* findInput( const std::string& inputName ) const;

   /**
    * Looks for the specified output, or returns NULL if one can't be found.
    *
    * NOTE: even though the method returns a pointer, the caller is not supposed to destroy
    *       the returned object!
    *
    * @param outputName
    */
   TOutputSocket* findOutput( const std::string& outputName ) const;

   // -------------------------------------------------------------------------
   // Observer implementation
   // -------------------------------------------------------------------------
   void update( GBNodeSocket& subject );
   void update( GBNodeSocket& subject, const GBNodeSocketOperation& msg );

   // -------------------------------------------------------------------------
   // Sockets management
   // -------------------------------------------------------------------------
   /**
    * Defines a new node input.
    *
    * @param input
    */
   void defineInput( TInputSocket* input );

   /**
    * Removes an input with the specified name
    *
    * @param name
    */
   void removeInput( const std::string& name );

   /**
    * Defines a new node output.
    *
    * @param output
    */
   void defineOutput( TOutputSocket* output );

   /**
    * Returns the specified input.
    *
    * @param T             node input type
    * @param inputName
    */
   template< typename T >
   const T& getInput( const std::string& inputName ) const;

   /**
    * Returns the specified output.
    *
    * @param T             node output type
    * @param outputName
    */
   template< typename T >
   const T& getOutput( const std::string& outputName ) const;

protected:
   /**
    * Call this if you initialized the node's sockets in a bulk in the implementation.
    */
   void onBulkSocketsInitialization();
};

///////////////////////////////////////////////////////////////////////////////

#include "core/GraphBuilderNode.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_NODE_H
