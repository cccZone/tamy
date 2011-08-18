/// @file   dx9-Renderer/DX9LineSegmentsResourcesStorage.h
/// @brief  storage for line segments implementations
#pragma once

#include "core-Renderer\RenderResourceStorage.h"
#include "dx9-Renderer\DX9LineSegments.h"
#include "core-Renderer\LineSegments.h"


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

typedef RenderResourceStorage< DX9Renderer, LineSegments, DX9LineSegments > LineSegmentsStorage;

///////////////////////////////////////////////////////////////////////////////
