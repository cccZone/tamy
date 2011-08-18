/// @file   dx9-Renderer/DX9EffectsResourcesStorage.h
/// @brief  storage for effect shader implementations
#pragma once

#include "core-Renderer\RenderResourceStorage.h"
#include "core-Renderer/EffectShader.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

typedef RenderResourceStorage< DX9Renderer, EffectShader, ID3DXEffect > EffectsStorage;

///////////////////////////////////////////////////////////////////////////////
