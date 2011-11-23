/// @file   core/GraphBuilderSockets.h
/// @brief  graph builder node base sockets definitions.
#ifndef _GRAPH_BUILDER_SOCKETS_H
#define _GRAPH_BUILDER_SOCKETS_H


#include "core/Object.h"
#include "core/Subject.h"


///////////////////////////////////////////////////////////////////////////////

class GraphInstanceOwner;

///////////////////////////////////////////////////////////////////////////////

enum GBNodeSocketOperation
{
   GBNSO_CHANGED
};

///////////////////////////////////////////////////////////////////////////////

class GBNodeSocket : public Object, public Subject< GBNodeSocket, GBNodeSocketOperation >
{
protected:
   std::string                               m_name;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   GBNodeSocket( const std::string& name = "" );

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
template< typename NodeType >
class GBNodeOutput : public GBNodeSocket
{
protected:
   std::vector< NodeType* >     m_connectedNodes;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   GBNodeOutput( const std::string& name = "" );
   virtual ~GBNodeOutput();

   /**
    * Returns an array of nodes connected to this socket.
    */
   const std::vector< NodeType* >& getConnectedNodes() const { return m_connectedNodes; }

   /**
    * Connects the socket to the specified node.
    *
    * @param node
    */
   void connect( NodeType& node );

   /**
    * Disconnects the socket from the specified node.
    *
    * @param node
    */
   void disconnect( NodeType& node );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Input socket of a node.
 */
template< typename TOutputSocket >
class GBNodeInput : public GBNodeSocket
{
protected:
   TOutputSocket*             m_connectedOutput;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   GBNodeInput( const std::string& name = "" );
   virtual ~GBNodeInput();

   /**
    * Connects the socket to an output socket.
    * 
    * @param output
    *
    * @return     'true', if the connection was established successfully, 'false' otherwise
    */
   bool connect( TOutputSocket& output );

   /**
    * Disconnects the socket.
    */
   void disconnect();

protected:
   /**
    * Returns the output socket this socket is connected to, or NULL if the socket
    * isn't connected to anything.
    */
   inline const TOutputSocket* getOutput() const { return m_connectedOutput; }

   /**
    * Checks if the two sockets can be connected.
    *
    * @param output
    */
   virtual bool canConnect( TOutputSocket& output ) const { return false; }
};

///////////////////////////////////////////////////////////////////////////////

#include "core/GraphBuilderSockets.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_SOCKETS_H
