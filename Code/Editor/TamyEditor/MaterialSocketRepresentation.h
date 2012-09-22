/// @file   TamyEditor/MaterialSocketRepresentation.h
/// @brief  material pipeline's socket representation
#pragma once

#include "PipelineSocket.h"
#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

DECLARE_PIPELINE_SOCKET( MSSVec4, MaterialNode, 219, 255, 175 );
DECLARE_PIPELINE_SOCKET( MSSTexture, MaterialNode, 186, 166, 221 );
DECLARE_PIPELINE_SOCKET( MSSMatrix, MaterialNode, 159, 211, 55 );
DECLARE_PIPELINE_SOCKET( MSSFloat, MaterialNode, 163, 255, 247 );
DECLARE_PIPELINE_SOCKET( MSSBool, MaterialNode, 186, 211, 255 );
DECLARE_PIPELINE_SOCKET( MSSInt, MaterialNode, 255, 198, 237 );

///////////////////////////////////////////////////////////////////////////////
