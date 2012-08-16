/// @file   dx9-Renderer/DX9DepthBufferResourcesStorage.h
/// @brief  storage for depth buffer implementations
#pragma once

#include "core-Renderer\RenderResourceStorage.h"
#include "core-Renderer\DepthBuffer.h"
#include "dx9-Renderer\DX9DepthBuffer.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

typedef RenderResourceStorage< DX9Renderer, DepthBuffer, DX9DepthBuffer > DepthBuffersStorage;

///////////////////////////////////////////////////////////////////////////////
