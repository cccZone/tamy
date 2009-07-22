#pragma once

#include "core-Renderer\RenderingTargetsPolicy.h"


///////////////////////////////////////////////////////////////////////////////

/** 
 * This class allows to seamlessly switch between the used policies
 */
class RenderingTargetsPolicyProxy : public RenderingTargetsPolicy
{
private:
   RenderingTargetsPolicy* m_defaultPolicy;
   RenderingTargetsPolicy* m_currentPolicy;

public:
   RenderingTargetsPolicyProxy();
   ~RenderingTargetsPolicyProxy();

   void setPolicy(RenderingTargetsPolicy& policy);

   unsigned int setTargets(unsigned int passIdx);

   unsigned int getDefinedPassesCount() const;
};

///////////////////////////////////////////////////////////////////////////////
