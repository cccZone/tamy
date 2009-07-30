#include "core-Renderer\RenderingTechnique.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\RenderingTargetsPolicy.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

unsigned int RenderingTechnique::m_nextIndex = 0;

///////////////////////////////////////////////////////////////////////////////

RenderingTechnique::RenderingTechnique(const std::string& name, 
                                       RenderingTargetsPolicy& policy)
      : m_name(name),
      m_index(m_nextIndex),
      m_renderingTargetsPolicy(policy)
{
   ++m_nextIndex;
}

///////////////////////////////////////////////////////////////////////////////

RenderingTechnique::RenderingTechnique(const RenderingTechnique& rhs)
      : m_name(rhs.m_name),
      m_index(m_nextIndex),
      m_renderingTargetsPolicy(rhs.m_renderingTargetsPolicy)
{
   ++m_nextIndex;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingTechnique::render(Array<Renderable*>& renderables)
{
   unsigned int renderablesCount = renderables.size();

   unsigned int passesCount = beginRendering();
   for (unsigned int passIdx = 0; passIdx < passesCount; ++passIdx)
   {
      m_renderingTargetsPolicy.setTargets(passIdx);

      beginPass(passIdx);

      for (unsigned int i = 0; i < renderablesCount; ++i)
      {
         renderables[i]->render();
      }

      endPass(passIdx);
   }
   endRendering();
}

///////////////////////////////////////////////////////////////////////////////
