#include "core-Renderer\RenderingTechnique.h"
#include "core-Renderer\AbstractGraphicalNode.h"
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

void RenderingTechnique::render(Array<AbstractGraphicalNode*>& nodes, 
                                unsigned int startIdx, 
                                unsigned int endIdx)
{
   if (startIdx == endIdx) {return;}
   ASSERT(nodes.size() > startIdx, "Invalid start index");
   ASSERT(nodes.size() >= endIdx, "Invalid end index");

   unsigned int passesCount = beginRendering();

   for (unsigned int passIdx = 0; passIdx < passesCount; ++passIdx)
   {
      m_renderingTargetsPolicy.setTargets(passIdx);

      beginPass(passIdx);

      for (unsigned int nodeIdx = startIdx; nodeIdx < endIdx; ++nodeIdx)
      {
         nodes[nodeIdx]->render();
      }

      endPass(passIdx);
   }

   endRendering();
}

///////////////////////////////////////////////////////////////////////////////
