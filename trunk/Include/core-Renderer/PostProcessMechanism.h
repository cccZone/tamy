#pragma once

#include "core-Renderer\RenderingMechanism.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class PostProcessEffectNode;
class Renderable;
class RenderingTechnique;

///////////////////////////////////////////////////////////////////////////////

class PostProcessMechanism : public RenderingMechanism
{
private:
   PostProcessEffectNode* m_effect;
   RenderingTechnique& m_technique;
   Array<Renderable*> m_renderables;

public:
   PostProcessMechanism(RenderingTargetsPolicy* policy, 
                        RenderingTechnique& technique,
                        PostProcessEffectNode* effect);
   ~PostProcessMechanism();

   void render();
};

///////////////////////////////////////////////////////////////////////////////
