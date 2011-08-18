/// @file   dx9-Renderer/DX9VertexShaderResourcesStorage.h
/// @brief  storage for vertex shaders implementations
#pragma once

#include "core-Renderer\RenderResourceStorage.h"
#include "core-Renderer\VertexShader.h"
#include "dx9-Renderer\DX9VertexShader.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

typedef RenderResourceStorage< DX9Renderer, VertexShader, DX9VertexShader > VertexShadersStorage;

///////////////////////////////////////////////////////////////////////////////
