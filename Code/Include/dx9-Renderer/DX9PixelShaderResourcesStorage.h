/// @file   dx9-Renderer/DX9PixelShaderResourcesStorage.h
/// @brief  storage for pixel shader implementations
#pragma once

#include "core-Renderer\RenderResourceStorage.h"
#include "core-Renderer\PixelShader.h"
#include "dx9-Renderer\DX9PixelShader.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

typedef RenderResourceStorage< DX9Renderer, PixelShader, DX9PixelShader > PixelShadersStorage;

///////////////////////////////////////////////////////////////////////////////
