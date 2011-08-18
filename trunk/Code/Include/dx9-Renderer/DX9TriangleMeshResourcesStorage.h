/// @file   dx9-Renderer/DX9TriangleMeshResourcesStorage.h
/// @brief  storage for triangle meshes implementations
#pragma once

#include "core-Renderer\RenderResourceStorage.h"
#include "core-Renderer\TriangleMesh.h"
#include "dx9-Renderer\DX9TriangleMesh.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

typedef RenderResourceStorage< DX9Renderer, TriangleMesh, DX9TriangleMesh > TriangleMeshesStorage;

///////////////////////////////////////////////////////////////////////////////
