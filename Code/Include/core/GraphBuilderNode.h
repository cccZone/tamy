/// @file   core/GraphBuilderNode.h
/// @brief  basic graph builder node
#ifndef _GRAPH_BUILDER_NODE_H
#define _GRAPH_BUILDER_NODE_H

#include "core\MemoryRouter.h"
#include "core\Resource.h"
#include "core\Subject.h"
#include "core\Observer.h"
#include "core\GraphBuilderSockets.h"


///////////////////////////////////////////////////////////////////////////////

class GraphInstanceOwner;

///////////////////////////////////////////////////////////////////////////////

enum GraphBuilderNodeOperation
{
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
template< typename Impl >
class GraphBuilderNode : public Subject< Impl, GraphBuilderNodeOperation >
{
   DECLARE_ALLOCATOR( GraphBuilderNode, AM_ALIGNED_16 );

protected:
   typedef GBNodeInput< Impl > TInputSocket;
   typedef GBNodeOutput< Impl > TOutputSocket;
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

   /**
    * Checks if this node is directly connected with the specified node.
    *
    * @param otherNode
    */
   bool isConnectedWith( Impl* otherNode ) const;

   // -------------------------------------------------------------------------
   // Sockets management
   // -------------------------------------------------------------------------
   /**
    * Defines a single node input. 
    *
    * Don't use it if you define multiple inputs at once, as it'll generate a lot of update traffic.
    * Use 'defineInputs' instead.
    *
    * @param input
    */
   void defineInput( TInputSocket* input );

   /**
    * Defines node's inputs.
    *
    * @param inputs
    */
   void defineInputs( const std::vector< TInputSocket* >& inputs );

   /**
    * Re-defines node's inputs, removing the existing ones ( or preserving them if the supplied list
    * of inputs contains some that have the same name as the existing ones ), and creating new ones.
    *
    * Very useful if you would have to call removeAllInputs and then defineInputs, as it will
    * save you unnecessary update traffic.
    *
    * @param input
    */
   void redefineInputs( const std::vector< TInputSocket* >& inputs );

   /**
    * Removes all of node's inputs.
    */
   void removeAllInputs();

   /**
    * Removes selected inputs.
    *
    * @param inputNames
    */
   void removeSelectedInputs( const std::vector< std::string >& inputNames );

   /**
    * Defines a single node output. 
    *
    * Don't use it if you define multiple outputs at once, as it'll generate a lot of update traffic.
    * Use 'defineInputs' instead.
    *
    * @param output
    */
   void defineOutput( TOutputSocket* output );

   /**
    * Defines node's outputs.
    *
    * @param outputs
    */
   void defineOutputs( const std::vector< TOutputSocket* >& outputs );

   /**
    * Re-defines node's outputs, removing the existing ones ( or preserving them if the supplied list
    * of inputs contains some that have the same name as the existing ones ), and creating new ones.
    *
    * Very useful if you would have to call removeAllOutputs and then defineOutputs, as it will
    * save you unnecessary update traffic.
    *
    * @param outputs
    */
   void redefineOutputs( const std::vector< TOutputSocket* >& outputs );

   /**
    * Removes all of node's outputs.
    */
   void removeAllOutputs();

   /**
    * Removes selected outputs.
    *
    * @param outputNames
    */
   void removeSelectedOutputs( const std::vector< std::string >& outputNames );

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

   /**
    * Notifies the listeners that inputs connectivity has changed.
    */
   void notifyInputsChanged();

   /**
    * Notifies the listeners that outputs connectivity has changed.
    */
   void notifyOutputsChanged();

};

///////////////////////////////////////////////////////////////////////////////

#include "core/GraphBuilderNode.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_NODE_H
