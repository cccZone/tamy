/// @file   core-Renderer/RenderingPipelineSockets.h
/// @brief  rendering pipeline node base sockets definitions.
#pragma once

#include "core/Object.h"
#include "core/Subject.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineNode;
class RenderingPipelineMechanism;

///////////////////////////////////////////////////////////////////////////////

enum RPNodeSocketOperation
{
   RPNSO_CHANGED
};

///////////////////////////////////////////////////////////////////////////////

class RPNodeSocket : public Object, public Subject< RPNodeSocket, RPNodeSocketOperation >
{
   DECLARE_CLASS( RPNodeSocket )

private:
   std::string                               m_name;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   RPNodeSocket( const std::string& name = "" );

   /**
    * Returns the socket's name.
    */
   inline const std::string& getName() const { return m_name; }

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onPropertyChanged( Property& property );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Output socket of a node.
 */
class RPNodeOutput : public RPNodeSocket
{
   DECLARE_CLASS( RPNodeOutput )

private:
   std::vector< RenderingPipelineNode* >     m_connectedNodes;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   RPNodeOutput( const std::string& name = "" );
   virtual ~RPNodeOutput();

   /**
    * Returns an array of nodes connected to this socket.
    */
   const std::vector< RenderingPipelineNode* >& getConnectedNodes() const { return m_connectedNodes; }

   /**
    * Connects the socket to the specified node.
    *
    * @param node
    */
   void connect( RenderingPipelineNode& node );

   /**
    * Disconnects the socket from the specified node.
    *
    * @param node
    */
   void disconnect( RenderingPipelineNode& node );

   /**
    * Initializes runtime data layout for the socket.
    *
    * @param host    host mechanism
    */
   virtual void createLayout( RenderingPipelineMechanism& host ) const {}

   /**
    * Initializes the socket.
    *
    * @param host    host mechanism
    */
   virtual void initialize( RenderingPipelineMechanism& host ) const {}
   
   /**
    * Deinitializes the socket.
    *
    * @param host    host mechanism
    */
   void deinitialize( RenderingPipelineMechanism& host ) const {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Input socket of a node.
 */
class RPNodeInput : public RPNodeSocket
{
   DECLARE_CLASS( RPNodeInput )

private:
   RPNodeOutput*                             m_connectedOutput;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   RPNodeInput( const std::string& name = "" );
   virtual ~RPNodeInput();

   /**
    * Connects the socket to an output socket.
    * 
    * @param output
    *
    * @return     'true', if the connection was established successfully, 'false' otherwise
    */
   bool connect( RPNodeOutput& output );

   /**
    * Disconnects the socket.
    */
   void disconnect();

protected:
   /**
    * Returns the output socket this socket is connected to, or NULL if the socket
    * isn't connected to anything.
    */
   inline const RPNodeOutput* getOutput() const { return m_connectedOutput; }

   /**
    * Checks if the two sockets can be connected.
    *
    * @param output
    */
   virtual bool canConnect( RPNodeOutput& output ) const { return false; }
};

///////////////////////////////////////////////////////////////////////////////
