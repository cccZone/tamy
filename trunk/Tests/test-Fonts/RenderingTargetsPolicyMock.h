#pragma once

#include "core-Renderer\RenderingTargetsPolicy.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class RenderingTargetsPolicyMock : public RenderingTargetsPolicy
{
public:
   unsigned int setTargets(unsigned int passIdx) {return 0;}

   unsigned int getDefinedPassesCount() const {return 0;}
};

///////////////////////////////////////////////////////////////////////////////
