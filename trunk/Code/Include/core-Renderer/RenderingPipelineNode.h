/// @file   core-Renderer/RenderingPipelineNode.h
/// @brief  rendering pipeline node base
#ifndef _RENDERING_PIPELINE_NODE_H
#define _RENDERING_PIPELINE_NODE_H

#include "core/Resource.h"
#include "core/Subject.h"
#include "core/Observer.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineMechanism;
class RPNodeInput;
class RPNodeOutput;
class RPNodeSocket;
enum RPNodeSocketOperation;

///////////////////////////////////////////////////////////////////////////////

enum RenderingPipelineNodeOperation
{
   RPNO_CHANGED
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node.
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
class RenderingPipelineNode : public ResourceObject, 
                              public Subject< RenderingPipelineNode, RenderingPipelineNodeOperation >,
                              public Observer< RPNodeSocket, RPNodeSocketOperation >
{
   DECLARE_CLASS( RenderingPipelineNode )

private:
   typedef std::vector< RPNodeInput* >       InputsMap;
   typedef std::vector< RPNodeOutput* >      OutputsMap;

private:
   InputsMap                                 m_inputs;
   OutputsMap                                m_outputs;

public:
   virtual ~RenderingPipelineNode();

   /**
    * Called in order to initialize the node. This takes place before the pipeline
    * is first use in the rendering process.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   void initialize( RenderingPipelineMechanism& host );

   /**
    * Called in order to initialize the node. This takes place once
    * the entire pipeline stops being used for rendering and gets deinitialized.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   void deinitialize( RenderingPipelineMechanism& host );
   
   /**
    * Called in order to perform all rendering-related things.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   void update( RenderingPipelineMechanism& host );

   /**
    * Returns nodes that should be run after this node.
    *
    * @param outNodesToRun    an array to append the nodes to
    */
   void getSubsequentNodes( std::vector< RenderingPipelineNode* >& outNodesToRun ) const;

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
   bool connectToInput( RPNodeOutput& output, const std::string& inputName );

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
   void connectToOutput( RenderingPipelineNode& node, const std::string& outputName );

   /**
    * Disconnects a node from the specified output.
    *
    * @param node             connected node
    * @param outputName
    */
   void disconnectFromOutput( RenderingPipelineNode& node, const std::string& outputName );
   
   /**
    * Returns an array of all defined input sockets.
    */
   inline const std::vector< RPNodeInput* >& getInputs() const { return m_inputs; }

   /**
    * Returns an array of all defined output sockets.
    */
   inline const std::vector< RPNodeOutput* >& getOutputs() const { return m_outputs; }

   /**
    * Looks for the specified input, or returns NULL if one can't be found.
    *
    * NOTE: even though the method returns a pointer, the caller is not supposed to destroy
    *       the returned object!
    *
    * @param inputName
    */
   RPNodeInput* findInput( const std::string& inputName ) const;

   /**
    * Looks for the specified output, or returns NULL if one can't be found.
    *
    * NOTE: even though the method returns a pointer, the caller is not supposed to destroy
    *       the returned object!
    *
    * @param outputName
    */
   RPNodeOutput* findOutput( const std::string& outputName ) const;

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
   void onPropertyChanged( Property& property );

   // -------------------------------------------------------------------------
   // Observer implementation
   // -------------------------------------------------------------------------
   void update( RPNodeSocket& subject );
   void update( RPNodeSocket& subject, const RPNodeSocketOperation& msg );

protected:
   // -------------------------------------------------------------------------
   // Sockets management
   // -------------------------------------------------------------------------
   /**
    * Defines a new node input.
    *
    * @param input
    */
   void defineInput( RPNodeInput* input );

   /**
    * Defines a new node output.
    *
    * @param output
    */
   void defineOutput( RPNodeOutput* output );

   /**
    * Returns the specified input.
    *
    * @param T             node input type
    * @param inputName
    */
   template< typename T >
   T& getInput( const std::string& inputName );

   /**
    * Returns the specified output.
    *
    * @param T             node output type
    * @param outputName
    */
   template< typename T >
   T& getOutput( const std::string& outputName );

   /**
    * Called in order to initialize the node implementation. This takes place before the pipeline
    * is first use in the rendering process.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void onInitialize( RenderingPipelineMechanism& host ) {}

   /**
    * Called in order to initialize the node implementation. This takes place once
    * the entire pipeline stops being used for rendering and gets deinitialized.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void onDeinitialize( RenderingPipelineMechanism& host ) {}
   
   /**
    * Called in order to perform all rendering-related things the implementation is responsible for.
    *
    * @param host    pass from the context of which the rendering takes place
    */
   virtual void onUpdate( RenderingPipelineMechanism& host ) {}
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer/RenderingPipelineNode.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _RENDERING_PIPELINE_NODE_H
