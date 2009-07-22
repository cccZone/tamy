#pragma once

#include "core-Renderer\GraphicalEffect.h"


///////////////////////////////////////////////////////////////////////////////

class NullGraphicalEffect : public GraphicalEffect
{
public:
   NullGraphicalEffect() : GraphicalEffect("NullGraphicalEffect") {}

protected:
   void beginRendering() {}
   void endRendering(){}
};

///////////////////////////////////////////////////////////////////////////////
