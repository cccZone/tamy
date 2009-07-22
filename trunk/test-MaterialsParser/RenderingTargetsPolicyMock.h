#pragma once

#include "core-Renderer\RenderingTargetsPolicy.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class RenderingTargetsPolicyMock : public RenderingTargetsPolicy
{
public:
   void setTargets(unsigned int passIdx) {}
};

///////////////////////////////////////////////////////////////////////////////
