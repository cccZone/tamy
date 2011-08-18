/// @file   dx9-Renderer/DX9RenderTargetResourcesStorage.h
/// @brief  storage for render target implementations
#pragma once

#include "core-Renderer\RenderResourceStorage.h"
#include "core-Renderer\RenderTarget.h"
#include "dx9-Renderer\DX9RenderTarget.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

typedef RenderResourceStorage< DX9Renderer, RenderTarget, DX9RenderTarget > RenderTargetsStorage;

///////////////////////////////////////////////////////////////////////////////
