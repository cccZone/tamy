#include "RenderingPipelineSocketRepresentation.h"
#include "RenderingPipelineBlock.h"
#include "core-Renderer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( RenderingPipelineSocketRepresentation, GraphBlockSocket );
   PROPERTY( std::string, m_socketName );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineSocketRepresentation::RenderingPipelineSocketRepresentation()
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineSocketRepresentation::RenderingPipelineSocketRepresentation( GBNodeSocket& socket )
{
   m_socketName = socket.getName();
}

///////////////////////////////////////////////////////////////////////////////

Object& RenderingPipelineSocketRepresentation::getSocket()
{
   RenderingPipelineNode& parentNode = dynamic_cast< RenderingPipelineNode& >( getParentBlock().getNode() );
   GBNodeSocket* socket = parentNode.findInput( m_socketName );
   if ( !socket )
   {
      socket = parentNode.findOutput( m_socketName );
   }

   ASSERT_MSG( socket != NULL, "Socket not found" );
   return *socket;
}

///////////////////////////////////////////////////////////////////////////////

bool RenderingPipelineSocketRepresentation::onConnectionAdded( GraphBlockConnection& connection )
{
   RenderingPipelineNode& parentNode = dynamic_cast< RenderingPipelineNode& >( getParentBlock().getNode() );
   
   // handle disconnecting depending on the underlying socket type
   if ( &connection.getSource() == this )
   {
      RenderingPipelineNode& nextNode = dynamic_cast< RenderingPipelineNode& >( connection.getDestination().getParentBlock().getNode() );
      parentNode.connectToOutput( nextNode, m_socketName );
      return true;
   }
   else if ( &connection.getDestination() == this )
   {
      RPNodeOutput& otherOutput = dynamic_cast< RPNodeOutput& >( connection.getSource().getSocket() );
      return parentNode.connectToInput( otherOutput, m_socketName );
   }
   else
   {
      ASSERT_MSG( false, "Invalid socket encountered" );
      return false;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineSocketRepresentation::onConnectionRemoved( GraphBlockConnection& connection )
{
   RenderingPipelineNode& parentNode = dynamic_cast< RenderingPipelineNode& >( getParentBlock().getNode() );

   // handle connecting depending on the underlying socket type
   if ( &connection.getSource() == this )
   {
      // this is an output
      RenderingPipelineNode& nextNode = dynamic_cast< RenderingPipelineNode& >( connection.getDestination().getParentBlock().getNode() );
      parentNode.disconnectFromOutput( nextNode, m_socketName );
   }
   else if ( &connection.getDestination() == this )
   {
      parentNode.disconnectFromInput( m_socketName );
   }
   else
   {
      ASSERT_MSG( false, "Invalid socket encountered" );
   }
}

///////////////////////////////////////////////////////////////////////////////
