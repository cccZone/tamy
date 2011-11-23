/// @file   TamyEditor/RenderingPipelineSocketRepresentation.h
/// @brief  rendering pipeline's socket representation
#pragma once

#include "GraphBlock.h"
#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineBlock;

///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineSocketRepresentation : public GraphBlockSocket
{
   DECLARE_CLASS( RenderingPipelineSocketRepresentation )

private:
   std::string          m_socketName;

public:
   /**
    * Constructor called when the block is being deserialized.
    */
   RenderingPipelineSocketRepresentation();

   /**
    * Constructor called when the socket is created by a factory.
    *
    * @param socket        socket the node represents.
    */
   RenderingPipelineSocketRepresentation( GBNodeSocket& socket );

   /**
    * Returns the name of the represented socket.
    */
   inline const std::string& getSocketName() const { return m_socketName; }

   // -------------------------------------------------------------------------
   // GraphBlockSocket implementation
   // -------------------------------------------------------------------------
   Object& getSocket();

protected:
   bool onConnectionAdded( GraphBlockConnection& connection );
   void onConnectionRemoved( GraphBlockConnection& connection );
};

///////////////////////////////////////////////////////////////////////////////
