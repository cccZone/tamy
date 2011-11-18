/// @file   core-Renderer/RPSceneBuilder.h
/// @brief  base class for the rendering pipeline scene builders.
#pragma once

#include "core/Object.h"
#include "core-Renderer/SceneRenderTreeBuilder.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Base class for the scene builders that can be embedded in the scene rendering block.
 */
class RPSceneBuilder : public Object, public SceneRenderTreeBuilder
{
   DECLARE_CLASS( RPSceneBuilder )

public:
   virtual ~RPSceneBuilder() {}
};

///////////////////////////////////////////////////////////////////////////////
