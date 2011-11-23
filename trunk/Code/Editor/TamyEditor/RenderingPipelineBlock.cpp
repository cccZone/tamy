#include "RenderingPipelineBlock.h"

// sockets
#include "RPSVoid.h"
#include "RPSFloat.h"
#include "RPSTexture.h"
#include "RPSBool.h"
#include "RPSInt.h"
#include "RPSString.h"
#include "RPSVec4.h"
#include "RPSMatrix.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( RenderingPipelineBlock, GraphBlock )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineBlock::RenderingPipelineBlock()
   : m_socketsFactory( NULL )
{
   initSocketsFactory();
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineBlock::RenderingPipelineBlock( RenderingPipelineNode& node )
   : m_socketsFactory( NULL )
{
   initSocketsFactory();
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineBlock::~RenderingPipelineBlock()
{
   delete m_socketsFactory; m_socketsFactory = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineBlock::initSocketsFactory()
{
   // define associations
   m_socketsFactory = new GenericFactory< GBNodeSocket, GraphBlockSocket >();
   m_socketsFactory->associate< RPVoidInput, RPSVoid >();
   m_socketsFactory->associate< RPVoidOutput, RPSVoid >();
   m_socketsFactory->associate< RPTextureInput, RPSTextureInput >();
   m_socketsFactory->associate< RPRenderTargetOutput, RPSRenderTargetOutput >();
   m_socketsFactory->associate< RPImageOutput, RPSImageOutput >();
   m_socketsFactory->associate< RPFloatInput, RPSFloat >();
   m_socketsFactory->associate< RPFloatOutput, RPSFloat >();
   m_socketsFactory->associate< RPBoolInput, RPSBool >();
   m_socketsFactory->associate< RPBoolOutput, RPSBool >();
   m_socketsFactory->associate< RPIntInput, RPSInt >();
   m_socketsFactory->associate< RPIntOutput, RPSInt >();
   m_socketsFactory->associate< RPStringInput, RPSString >();
   m_socketsFactory->associate< RPStringOutput, RPSString >();
   m_socketsFactory->associate< RPVec4Input, RPSVec4 >();
   m_socketsFactory->associate< RPVec4Output, RPSVec4 >();
   m_socketsFactory->associate< RPMatrixInput, RPSMatrix >();
   m_socketsFactory->associate< RPMatrixOutput, RPSMatrix >();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineBlock::refreshInputs( RenderingPipelineNode& node )
{
   // first - gather the names of all inputs the block currently has
   std::set< std::string > inputNames;
   getAllSockets( GBSP_INPUT, inputNames );

   // now go through the node's inputs and see which ones were just created
   const std::vector< RPNodeInput* >& inputs = node.getInputs();
   for ( std::vector< RPNodeInput* >::const_iterator it = inputs.begin(); it != inputs.end(); ++it )
   {
      // check if an input with this name doesn't already exist - if it does, don't create one
      const std::string& inputName = (*it)->getName();
      if ( inputNames.find( inputName ) == inputNames.end() )
      {
         GraphBlockSocket* socket = m_socketsFactory->create( **it );
         socket->initialize( this, GBSP_INPUT, inputName.c_str() );
         addSocket( socket );
      }

      inputNames.erase( inputName );
   }

   // now go through the remaining input names - those are the inputs that were deleted - and remove them
   removeSockets( GBSP_INPUT, inputNames );
}

///////////////////////////////////////////////////////////////////////////////
   
void RenderingPipelineBlock::refreshOutputs( RenderingPipelineNode& node )
{
   // first - gather the names of all outputs the block currently has
   std::set< std::string > outputNames;
   getAllSockets( GBSP_OUTPUT, outputNames );

   // now go through the node's outputs and see which ones were just created
   const std::vector< RPNodeOutput* >& outputs = node.getOutputs();
   for ( std::vector< RPNodeOutput* >::const_iterator it = outputs.begin(); it != outputs.end(); ++it )
   {
      // check if an input with this name doesn't already exist - if it does, don't create one
      const std::string& outputName = (*it)->getName();
      if ( outputNames.find( outputName ) == outputNames.end() )
      {
         GraphBlockSocket* socket = m_socketsFactory->create( **it );
         socket->initialize( this, GBSP_OUTPUT, outputName.c_str() );
         addSocket( socket );
      }

      outputNames.erase( outputName );
   }

   // now go through the remaining output names - those are the inputs that were deleted - and remove them
   removeSockets( GBSP_OUTPUT, outputNames );
}

///////////////////////////////////////////////////////////////////////////////
