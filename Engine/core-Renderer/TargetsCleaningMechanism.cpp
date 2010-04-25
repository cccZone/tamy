#include "core-Renderer\TargetsCleaningMechanism.h"
#include "core-Renderer\RenderingTargetsCleaner.h"
#include "core-Renderer\RenderingTargetsPolicy.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

TargetsCleaningMechanism::TargetsCleaningMechanism(RenderingTargetsPolicy* policy, 
                                                   RenderingTargetsCleaner* cleaner)
: m_policy(policy)
, m_cleaner(cleaner)
{
}

///////////////////////////////////////////////////////////////////////////////

TargetsCleaningMechanism::~TargetsCleaningMechanism()
{
   delete m_policy;
   m_policy = NULL;

   delete m_cleaner;
   m_cleaner = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TargetsCleaningMechanism::render()
{
   unsigned int renderingPassesCount = m_policy->getDefinedPassesCount();
   if (renderingPassesCount == 0) {return;}

   unsigned int targetsCount = 0;
   for (unsigned int passIdx = 0; passIdx < renderingPassesCount; ++passIdx)
   {
      targetsCount = m_policy->setTargets(passIdx);
      m_cleaner->cleanRenderingTargets(targetsCount);
   }
}

///////////////////////////////////////////////////////////////////////////////
