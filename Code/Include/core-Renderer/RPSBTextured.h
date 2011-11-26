/// @file   core-Renderer/RPSBTextured.h
/// @brief  use this builder if you want to render the scene as it should be seen
#pragma once

#include "core-Renderer/RPSceneBuilder.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Use this builder if you want to render the scene as it should be seen.
 */
class RPSBTextured : public RPSceneBuilder
{
   DECLARE_CLASS( RPSBTextured )

public:
   // -------------------------------------------------------------------------
   // SceneRenderTreeBuilder implementation
   // -------------------------------------------------------------------------
   StateTreeNode* buildRenderTree( MemoryPool& pool, const Array< Geometry* >& visibleElems, RuntimeDataBuffer& data ) const;
};

///////////////////////////////////////////////////////////////////////////////
