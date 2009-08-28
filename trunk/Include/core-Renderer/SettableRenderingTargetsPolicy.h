#pragma once

#include "core-Renderer\RenderingTargetsPolicy.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class RenderingTarget;

///////////////////////////////////////////////////////////////////////////////

/**
 * Default policy implementation that allows to define what
 * targets the policy is composed of at runtime.
 */
class SettableRenderingTargetsPolicy : public RenderingTargetsPolicy
{
private:
   std::vector<std::vector<RenderingTarget*> > m_targets;

public:
   virtual ~SettableRenderingTargetsPolicy() {}
   /**
    * Adds a new rendering target for the specified rendering pass.
    *
    * @param passIdx    rendering pass index
    * @param target     rendering target we want to use for the pass
    */
   void addTarget(unsigned int passIdx, RenderingTarget& target);

   /**
    * Removes a rendering tareget from the specified rendering pass.
    *
    * @param passIdx    rendering pass index
    * @param target     rendering target we don't want to use any more
    */
   void removeTarget(unsigned int passIdx, RenderingTarget& target);

   unsigned int setTargets(unsigned int passIdx);

   unsigned int getDefinedPassesCount() const {return m_targets.size();}
};

///////////////////////////////////////////////////////////////////////////////
