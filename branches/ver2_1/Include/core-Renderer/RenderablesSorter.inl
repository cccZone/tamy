#ifndef _RENDERABLES_SORTER_H
#error "This file can only be included from RenderablesSorter.h"
#else

#include "core-Renderer\Material.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

template <typename InRenderable>
void RenderablesSorter<InRenderable>::operator()(
                                         typename const Array<InRenderable*>& input, 
                                         const Frustum& frustum,
                                         const D3DXVECTOR3& cameraGlobalPos,
                                         Array<Renderable*>& output)
{
   // segregate nodes
   output.clear();

   unsigned int nodesCount = input.size();
   output.resize(nodesCount, NULL);

   unsigned int regularNodeIdx = 0;
   unsigned int transparentNodeIdx = nodesCount - 1;
   for (unsigned int i = 0; i < nodesCount; ++i)
   {
      if (input[i]->getMaterial().isTransparent() == false)
      {
         output[regularNodeIdx] = input[i];
         ++regularNodeIdx;
      }
      else
      {
         output[transparentNodeIdx] = input[i];
         --transparentNodeIdx;
      }
   }

   // sort the nodes
   std::sort((Renderable**)output, 
      (Renderable**)output + regularNodeIdx,
      m_materialsComparator);


   m_distanceComparator.setReferencePoint(cameraGlobalPos);
   std::sort((Renderable**)output + regularNodeIdx, 
      (Renderable**)output + output.size(),
      m_distanceComparator);
}

///////////////////////////////////////////////////////////////////////////////

#endif // _RENDERABLES_SORTER_H
