/// @file   TamyEditor/PipelineSocket.h
/// @brief  pipeline socket representation
#ifndef _PIPELINE_SOCKET_H
#define _PIPELINE_SOCKET_H

#include "GraphBlock.h"


///////////////////////////////////////////////////////////////////////////////

class GBNodeSocket;

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
class PipelineSocket : public GraphBlockSocket
{
public:
   /**
    * Constructor called when the block is being deserialized.
    */
   PipelineSocket();

   /**
    * Constructor called when the socket is created by a factory.
    *
    * @param socket        socket the node represents.
    */
   PipelineSocket( GBNodeSocket& socket );

   // -------------------------------------------------------------------------
   // GraphBlockSocket implementation
   // -------------------------------------------------------------------------
   Object& getSocket();

protected:
   bool onConnectionAdded( GraphBlockConnection& connection );
   void onConnectionRemoved( GraphBlockConnection& connection );
};

///////////////////////////////////////////////////////////////////////////////

#include "PipelineSocket.inl"

///////////////////////////////////////////////////////////////////////////////

#define DECLARE_PIPELINE_SOCKET( Class, NodeType, r, g, b )                   \
class Class : public PipelineSocket< NodeType >                               \
{                                                                             \
   DECLARE_CLASS( Class )                                                     \
                                                                              \
public:                                                                       \
   Class() : PipelineSocket< NodeType >() {}                                  \
   Class( GBNodeSocket& socket ) : PipelineSocket< NodeType >( socket ) {}    \
   QColor getBgColor() const { return QColor( r, g, b ); }                    \
};

///////////////////////////////////////////////////////////////////////////////


#endif // _PIPELINE_SOCKET_H
