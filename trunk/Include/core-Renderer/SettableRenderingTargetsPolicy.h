#pragma once

#include "core-Renderer\RenderingTargetsPolicy.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class RenderingTarget;

///////////////////////////////////////////////////////////////////////////////

class SettableRenderingTargetsPolicy : public RenderingTargetsPolicy
{
private:
   std::vector<std::vector<RenderingTarget*> > m_targets;

public:
   void addTarget(unsigned int passIdx, RenderingTarget& target);

   void removeTarget(unsigned int passIdx, RenderingTarget& target);

   unsigned int setTargets(unsigned int passIdx);

   unsigned int getDefinedPassesCount() const {return m_targets.size();}
};

///////////////////////////////////////////////////////////////////////////////
