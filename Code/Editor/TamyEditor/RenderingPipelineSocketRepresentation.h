/// @file   TamyEditor/RenderingPipelineSocketRepresentation.h
/// @brief  rendering pipeline's socket representation
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"
#include "GraphBlockSocket.h"


///////////////////////////////////////////////////////////////////////////////

DECLARE_PIPELINE_SOCKET( RPSVoid, RenderingPipelineNode, 224, 224, 166 );
DECLARE_PIPELINE_SOCKET( RPSVec4, RenderingPipelineNode, 219, 255, 175 );
DECLARE_PIPELINE_SOCKET( RPSTexture, RenderingPipelineNode, 186, 166, 221 );
DECLARE_PIPELINE_SOCKET( RPSString, RenderingPipelineNode, 255, 209, 132 );
DECLARE_PIPELINE_SOCKET( RPSMatrix, RenderingPipelineNode, 159, 211, 55 );
DECLARE_PIPELINE_SOCKET( RPSInt, RenderingPipelineNode, 255, 198, 237 );
DECLARE_PIPELINE_SOCKET( RPSFloat, RenderingPipelineNode, 163, 255, 247 );
DECLARE_PIPELINE_SOCKET( RPSBool, RenderingPipelineNode, 186, 211, 255 );

///////////////////////////////////////////////////////////////////////////////