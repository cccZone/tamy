#pragma once

/// @file   ext-RendererView\SceneManagers.h
/// @brief  declarations of scene managers types

#include "core\LinearStorage.h"
#include "core\TNodesSpatialStorage.h"
#include "core\StaticGeometryOctree.h"


///////////////////////////////////////////////////////////////////////////////

class RenderableNode;
class StaticGeometryRenderable;

///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

typedef TNodesSpatialStorage<RenderableNode, LinearStorage> DynamicSceneManager;
typedef StaticGeometryOctree<StaticGeometryRenderable> StaticSceneManager;

///////////////////////////////////////////////////////////////////////////////

} // RendererView
