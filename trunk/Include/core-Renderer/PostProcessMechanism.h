#pragma once

#include "core-Renderer\RenderingMechanism.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class PostProcessEffectRenderable;
class Renderable;
class RenderingTechnique;

///////////////////////////////////////////////////////////////////////////////

class PostProcessMechanism : public RenderingMechanism
{
private:
   PostProcessEffectRenderable* m_effect;
   RenderingTechnique& m_technique;
   Array<Renderable*> m_renderables;

public:
   PostProcessMechanism(RenderingTargetsPolicy* policy, 
                        RenderingTechnique& technique,
                        PostProcessEffectRenderable* effect);
   ~PostProcessMechanism();

   void render();
};

///////////////////////////////////////////////////////////////////////////////
