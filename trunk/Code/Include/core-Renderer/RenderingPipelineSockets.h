/// @file   core-Renderer/RenderingPipelineSockets.h
/// @brief  rendering pipeline node base sockets definitions.
#pragma once

#include "core/GraphBuilderSockets.h"
#include "core/Object.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineMechanism;
class RenderingPipelineNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Output socket of a node.
 */
class RPNodeOutput : public GBNodeOutput< RenderingPipelineNode >
{
   DECLARE_CLASS( RPNodeOutput )

public:
   /**
    * Constructor.
    *
    * @param name
    */
   RPNodeOutput( const std::string& name = "" );
   virtual ~RPNodeOutput();

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
class RPNodeInput : public GBNodeInput< RPNodeOutput >
{
   DECLARE_CLASS( RPNodeInput )

public:
   /**
    * Constructor.
    *
    * @param name
    */
   RPNodeInput( const std::string& name = "" );
   virtual ~RPNodeInput();
};

///////////////////////////////////////////////////////////////////////////////
