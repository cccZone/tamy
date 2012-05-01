#ifndef _PIPELINE_SOCKET_H
#error "This file can only be included from PipelineSocket.h"
#else

#include "core/GraphBuilderNode.h"


///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PipelineSocket< TNode >::PipelineSocket()
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PipelineSocket< TNode >::PipelineSocket( GBNodeSocket& socket )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
ReflectionObject& PipelineSocket< TNode >::getSocket()
{
   TNode& parentNode = dynamic_cast< TNode& >( getParentBlock().getNode() );
   GBNodeSocket* socket = parentNode.findInput( getName() );
   if ( !socket )
   {
      socket = parentNode.findOutput( getName() );
   }

   ASSERT_MSG( socket != NULL, "Socket not found" );
   return *socket;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
bool PipelineSocket< TNode >::onConnectionAdded( GraphBlockConnection& connection )
{   
   // handle connecting depending on the underlying socket type
   if ( &connection.getSource() == this )
   {
      TNode& sourceNode = dynamic_cast< TNode& >( connection.getSource().getParentBlock().getNode() );
      TNode& destNode = dynamic_cast< TNode& >( connection.getDestination().getParentBlock().getNode() );

      return sourceNode.connect( connection.getSource().getName(), destNode, connection.getDestination().getName() );
   }

   return true;

}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PipelineSocket< TNode >::onConnectionRemoved( GraphBlockConnection& connection )
{
   // handle disconnecting depending on the underlying socket type
   if ( &connection.getSource() == this )
   {
      TNode& sourceNode = dynamic_cast< TNode& >( connection.getSource().getParentBlock().getNode() );
      TNode& destNode = dynamic_cast< TNode& >( connection.getDestination().getParentBlock().getNode() );

      sourceNode.disconnect( destNode, connection.getDestination().getName() );
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PIPELINE_SOCKET_H
