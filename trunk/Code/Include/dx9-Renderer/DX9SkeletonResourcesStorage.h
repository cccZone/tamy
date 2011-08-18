/// @file   dx9-Renderer/DX9SkeletonResourcesStorage.h
/// @brief  storage for skeleton vertex buffers
#pragma once

#include "core-Renderer\RenderResourceStorage.h"
#include "core-Renderer\Skeleton.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

typedef RenderResourceStorage< DX9Renderer, Skeleton, IDirect3DVertexBuffer9 > SkeletonsStorage;

///////////////////////////////////////////////////////////////////////////////
