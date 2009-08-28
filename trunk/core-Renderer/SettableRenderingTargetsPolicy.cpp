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

   // check if this target instance is already added
   unsigned int count = targetsForPass.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if (targetsForPass[i] == &target) {return;}
   }

   targetsForPass.push_back(&target);
}

///////////////////////////////////////////////////////////////////////////////

void SettableRenderingTargetsPolicy::removeTarget(unsigned int passIdx, 
                                                  RenderingTarget& target)
{
   if (m_targets.size() <= passIdx) {return;}
   std::vector<RenderingTarget*>& targetsForPass = m_targets[passIdx];

   for (std::vector<RenderingTarget*>::iterator it = targetsForPass.begin();
        it != targetsForPass.end(); ++it)
   {
      if (*it == &target)
      {
         targetsForPass.erase(it);
         break;
      }
   }
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

