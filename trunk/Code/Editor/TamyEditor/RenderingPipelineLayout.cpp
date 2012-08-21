#include "RenderingPipelineLayout.h"
#include "core.h"
#include "core-Renderer.h"

// blocks & sockets
#include "RenderingPipelineSocketRepresentation.h"
#include "RPBPostProcess.h"
#include "RPBSceneRender.h"
#include "RPBStart.h"
#include "RPBAdapter.h"
#include "RPBFloats2Vec4.h"
#include "RPBCamera.h"
#include "RPBTexture.h"
#include "RPBFloat.h"
#include "RPBVec4.h"
#include "RPBDeferredLighting.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( RenderingPipelineLayout, rpl, AM_BINARY );
   PROPERTY( RenderingPipeline*, m_pipeline );
   PROPERTY( std::vector< GraphBlock* >, m_blocks );
   PROPERTY( std::vector< GraphBlockConnection* >, m_connections );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineLayout::RenderingPipelineLayout( const FilePath& resourceName )
   : PipelineLayout< RenderingPipeline, RenderingPipelineNode >( resourceName )
{
   // create block-to-node associations
   initBlocksFactory();

   // customize the looks
   setBackgroundBrush( QColor( 209, 226, 165 ) );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineLayout::initBlocksFactory()
{
   // create block-to-node associations
   associate< RPSceneRenderNode, RPBSceneRender >();
   associate< RPStartNode, RPBStart >();
   associate< RPAdapterNode, RPBAdapter >();
   associate< RPPostProcessNode, RPBPostProcess >();
   associate< RPFloats2Vec4Node, RPBFloats2Vec4 >();
   associate< RPCameraNode, RPBCamera >();
   associate< RPTextureNode, RPBTexture >();
   associate< RPFloatNode, RPBFloat >();
   associate< RPVec4Node, RPBVec4 >();
   associate< RPDeferredLightingNode, RPBDeferredLighting >();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineLayout::initSocketsFactory( PipelineBlock& block )
{
   GenericFactory< GBNodeSocket, GraphBlockSocket >& factory = block.getSocketsFactory();
   factory.associate< RPVoidInput, RPSVoid >();
   factory.associate< RPVoidOutput, RPSVoid >();
   factory.associate< RPTextureInput, RPSTexture >();
   factory.associate< RPTextureOutput, RPSTexture >();
   factory.associate< RPFloatInput, RPSFloat >();
   factory.associate< RPFloatOutput, RPSFloat >();
   factory.associate< RPBoolInput, RPSBool >();
   factory.associate< RPBoolOutput, RPSBool >();
   factory.associate< RPIntInput, RPSInt >();
   factory.associate< RPIntOutput, RPSInt >();
   factory.associate< RPStringInput, RPSString >();
   factory.associate< RPStringOutput, RPSString >();
   factory.associate< RPVec4Input, RPSVec4 >();
   factory.associate< RPVec4Output, RPSVec4 >();
   factory.associate< RPMatrixInput, RPSMatrix >();
   factory.associate< RPMatrixOutput, RPSMatrix >();
}

///////////////////////////////////////////////////////////////////////////////
