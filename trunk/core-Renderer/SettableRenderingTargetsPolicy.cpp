#include "core-Renderer\SettableRenderingTargetsPolicy.h"
#include "core-Renderer\RenderingTarget.h"


///////////////////////////////////////////////////////////////////////////////

void SettableRenderingTargetsPolicy::addTarget(unsigned int passIdx, 
                                               RenderingTarget& target)
{
   if (m_targets.size() < (passIdx + 1))
   {
      m_targets.resize(passIdx + 1);
   }
   std::vector<RenderingTarget*>& targetsForPass = m_targets[passIdx];
   targetsForPass.push_back(&target);
}

///////////////////////////////////////////////////////////////////////////////

unsigned int SettableRenderingTargetsPolicy::setTargets(unsigned int passIdx)
{
   if (m_targets.size() <= passIdx) {return 0;}
   std::vector<RenderingTarget*>& targetsForPass = m_targets[passIdx];

   unsigned int count = targetsForPass.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      targetsForPass[i]->use(i);
   }

   return count;
}

///////////////////////////////////////////////////////////////////////////////
