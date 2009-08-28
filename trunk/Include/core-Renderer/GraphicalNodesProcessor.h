#pragma once

#include <d3dx9.h>
#include "core\Array.h"
#include "core-Renderer\DistanceComparator.h"
#include "core-Renderer\BatchingStrategy.h"


///////////////////////////////////////////////////////////////////////////////

class RenderableNode;
class Renderable;
struct Frustum;

///////////////////////////////////////////////////////////////////////////////

/**
 * The functor's task is to segregate non-transparent & transparent
 * nodes in a way that transparent ones are rendered last and that
 * the rendering can be performed using batches of the same materials.
 */
class GraphicalNodesProcessor
{
private:
   BatchComparator m_materialsComparator;
   DistanceComparator m_distanceComparator;

public:
   void operator()(const Array<RenderableNode*>& input, 
                   const Frustum& frustum,
                   const D3DXVECTOR3& cameraGlobalPos,
                   Array<Renderable*>& output);
};

///////////////////////////////////////////////////////////////////////////////
