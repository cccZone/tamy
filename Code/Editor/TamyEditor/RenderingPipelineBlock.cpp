#include "RenderingPipelineBlock.h"
#include "core.h"

// sockets
#include "RPSVoid.h"
#include "RPSFloat.h"
#include "RPSTexture.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( RenderingPipelineBlock, GraphBlock )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineBlock::RenderingPipelineBlock( RenderingPipelineNode& node )
{
   // define associations
   GenericFactory< RPNodeSocket, GraphBlockSocket > socketFactory;
   socketFactory.associate< RPVoidInput, RPSVoid >();
   socketFactory.associate< RPVoidOutput, RPSVoid >();
   socketFactory.associate< RPTextureInput, RPSTextureInput >();
   socketFactory.associate< RPTextureOutput, RPSTextureOutput >();
   socketFactory.associate< RPFloatInput, RPSFloat >();
   socketFactory.associate< RPFloatOutput, RPSFloat >();

   // create input sockets
   const std::vector< RPNodeInput* >& inputs = node.getInputs();
   for ( std::vector< RPNodeInput* >::const_iterator it = inputs.begin(); it != inputs.end(); ++it )
   {
      GraphBlockSocket* socket = socketFactory.create( **it );
      socket->initialize( this, GBSP_INPUT, (*it)->getName().c_str() );
      addSocket( socket );
   }

   // create output sockets
   const std::vector< RPNodeOutput* >& outputs = node.getOutputs();
   for ( std::vector< RPNodeOutput* >::const_iterator it = outputs.begin(); it != outputs.end(); ++it )
   {
      GraphBlockSocket* socket = socketFactory.create( **it );
      socket->initialize( this, GBSP_OUTPUT, (*it)->getName().c_str() );
      addSocket( socket );
   }
}

///////////////////////////////////////////////////////////////////////////////
