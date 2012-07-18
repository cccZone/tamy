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
ReflectionObject* PipelineSocket< TNode >::getSocket()
{
   GraphBlock* parentBlock = getParentBlock();
   if ( !parentBlock )
   {
      return NULL;
   }

   TNode* parentNode = dynamic_cast< TNode* >( parentBlock->getNode() );
   if ( !parentNode )
   {
      return NULL;
   }

   GBNodeSocket* socket = parentNode->findInput( getName() );
   if ( !socket )
   {
      socket = parentNode->findOutput( getName() );
   }

   ASSERT_MSG( socket != NULL, "Socket not found" );
   return socket;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
bool PipelineSocket< TNode >::onConnectionAdded( GraphBlockConnection& connection )
{   
   // handle connecting depending on the underlying socket type
   if ( &connection.getSource() == this )
   {
      GraphBlock* sourceParentBlock = connection.getSource().getParentBlock();
      GraphBlock* destParentBlock = connection.getDestination().getParentBlock();

      if ( sourceParentBlock && destParentBlock )
      {
         TNode* sourceNode = dynamic_cast< TNode* >( sourceParentBlock->getNode() );
         TNode* destNode = dynamic_cast< TNode* >( destParentBlock->getNode() );

         return sourceNode->connect( connection.getSource().getName(), *destNode, connection.getDestination().getName() );
      }
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
      GraphBlock* sourceParentBlock = connection.getSource().getParentBlock();
      GraphBlock* destParentBlock = connection.getDestination().getParentBlock();

      if ( sourceParentBlock && destParentBlock )
      {
         TNode* sourceNode = dynamic_cast< TNode* >( sourceParentBlock->getNode() );
         TNode* destNode = dynamic_cast< TNode* >( destParentBlock->getNode() );
         sourceNode->disconnect( *destNode, connection.getDestination().getName() );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PIPELINE_SOCKET_H
