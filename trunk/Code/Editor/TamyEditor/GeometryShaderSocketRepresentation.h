/// @file   TamyEditor/GeometryShaderSocketRepresentation.h
/// @brief  geometry shader pipeline's socket representation
#pragma once

#include "GraphBlockSocket.h"
#include "core-Renderer/GeometryShaderNode.h"


///////////////////////////////////////////////////////////////////////////////

DECLARE_PIPELINE_SOCKET( GSSVec4, GeometryShaderNode, 219, 255, 175 );
DECLARE_PIPELINE_SOCKET( GSSTexture, GeometryShaderNode, 186, 166, 221 );
DECLARE_PIPELINE_SOCKET( GSSMatrix, GeometryShaderNode, 159, 211, 55 );
DECLARE_PIPELINE_SOCKET( GSSFloat, GeometryShaderNode, 163, 255, 247 );
DECLARE_PIPELINE_SOCKET( GSSBool, GeometryShaderNode, 186, 211, 255 );

///////////////////////////////////////////////////////////////////////////////
