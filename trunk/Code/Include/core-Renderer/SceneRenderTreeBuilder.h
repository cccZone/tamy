/// @file   core-Renderer/SceneRenderTreeBuilder.h
/// @brief  a strategy for building a scene tree for rendering
#pragma once

#include "core/Array.h"
#include "core-Renderer/RenderTree.h"


///////////////////////////////////////////////////////////////////////////////

class RuntimeDataBuffer;
class MemoryPool;

///////////////////////////////////////////////////////////////////////////////

/**
 * A strategy for building a scene tree for rendering.
 */
class SceneRenderTreeBuilder
{
public:
   virtual ~SceneRenderTreeBuilder() {}

   /**
    * Build the tree here.
    *
    * @param pool             memory pool in which we can allocate the tree nodes
    * @param visibleElems     array of visible elements we want to render.
    * @param data             runtime data buffer
    */
   virtual StateTreeNode* buildRenderTree( MemoryPool& pool, const Array< Geometry* >& visibleElems, RuntimeDataBuffer& data ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
