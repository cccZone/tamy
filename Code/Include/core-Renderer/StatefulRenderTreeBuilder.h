/// @file   core-Renderer/StatefulRenderTreeBuilder.h
/// @brief  creates a render tree by dividing entities with respect to the render states they use to optimize the rendering process
#pragma once

#include "core/Array.h"


///////////////////////////////////////////////////////////////////////////////

class Geometry;
class MemoryPool;
struct StateTreeNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Creates a render tree by dividing entities with respect to the render states they use to optimize the rendering process.
 */
class StatefulRenderTreeBuilder
{
public:
   /**
    * Build the render tree.
    */
   static StateTreeNode* buildRenderTree( MemoryPool& pool, const Array< Geometry* >& visibleElems );
};

///////////////////////////////////////////////////////////////////////////////
