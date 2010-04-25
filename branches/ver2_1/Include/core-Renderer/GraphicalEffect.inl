#ifndef _CORERENDERER_GRAPHICALEFFECT_H
#error "This file can only be included from GraphicalEffect.h"
#else

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void GraphicalEffect::render(T& renderable, 
                             RenderingTargetsPolicy& policy)
{
   unsigned int passesCount = beginRendering();
   for (unsigned int passIdx = 0; passIdx < passesCount; ++passIdx)
   {
      policy.setTargets(passIdx);

      beginPass(passIdx);
      renderable.render();
      endPass(passIdx);
   }
   endRendering();
}

///////////////////////////////////////////////////////////////////////////////

#endif //_CORERENDERER_GRAPHICALEFFECT_H
