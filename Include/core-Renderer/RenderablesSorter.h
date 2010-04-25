#ifndef _RENDERABLES_SORTER_H
#define _RENDERABLES_SORTER_H

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
 * renderables in a way that transparent ones are rendered last and that
 * the rendering can be performed using batches of the same materials.
 */
template <typename InRenderable>
class RenderablesSorter
{
private:
   BatchComparator m_materialsComparator;
   DistanceComparator m_distanceComparator;

public:
   void operator()(typename const Array<InRenderable*>& input, 
                   const Frustum& frustum,
                   const D3DXVECTOR3& cameraGlobalPos,
                   Array<Renderable*>& output);
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\RenderablesSorter.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _RENDERABLES_SORTER_H
