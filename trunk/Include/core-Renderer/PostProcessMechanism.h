#pragma once

#include "core-Renderer\RenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

class PostProcessEffectRenderable;
class GraphicalEffect;

///////////////////////////////////////////////////////////////////////////////

class PostProcessMechanism : public RenderingMechanism
{
private:
   PostProcessEffectRenderable* m_renderable;
   GraphicalEffect& m_effect;

public:
   PostProcessMechanism(RenderingTargetsPolicy* policy, 
                        GraphicalEffect& effect,
                        PostProcessEffectRenderable* renderable);
   ~PostProcessMechanism();

   void render();
};

///////////////////////////////////////////////////////////////////////////////
