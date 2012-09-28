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
#include "RPBLightIndices.h"
#include "RPBPreview.h"
#include "RPBMaterialsDB.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( RenderingPipelineLayout, rpl, AM_BINARY );
   PROPERTY( RenderingPipeline*, m_pipeline );
   PROPERTY( std::vector< GraphBlock* >, m_blocks );
   PROPERTY( std::vector< GraphBlockConnection* >, m_connections );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineLayout::RenderingPipelineLayout( const FilePath& resourceName )
   : TGraphLayout< RenderingPipeline, RenderingPipelineNode >( resourceName )
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineLayout::initBlocksFactory( BlocksFactory& factory )
{
   // create block-to-node associations
   factory.associate< RPSceneRenderNode, RPBSceneRender >();
   factory.associate< RPStartNode, RPBStart >();
   factory.associate< RPAdapterNode, RPBAdapter >();
   factory.associate< RPPostProcessNode, RPBPostProcess >();
   factory.associate< RPFloats2Vec4Node, RPBFloats2Vec4 >();
   factory.associate< RPCameraNode, RPBCamera >();
   factory.associate< RPTextureNode, RPBTexture >();
   factory.associate< RPFloatNode, RPBFloat >();
   factory.associate< RPVec4Node, RPBVec4 >();
   factory.associate< RPDeferredLightingNode, RPBDeferredLighting >();
   factory.associate< RPLightIndicesNode, RPBLightIndices >();
   factory.associate< RPPreviewNode, RPBPreview >();
   factory.associate< RPMaterialsDBNode, RPBMaterialsDB >();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineLayout::initSocketsFactory( SocketsFactory& factory )
{
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
