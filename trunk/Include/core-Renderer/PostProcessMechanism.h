#pragma once

#include "core-Renderer\RenderingMechanism.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class PostProcessEffectNode;
class AbstractGraphicalNode;

///////////////////////////////////////////////////////////////////////////////

class PostProcessMechanism : public RenderingMechanism
{
private:
   PostProcessEffectNode* m_effect;
   Array<AbstractGraphicalNode*> m_nodesToRender;

public:
   PostProcessMechanism(RenderingTargetsPolicy* policy, 
                        PostProcessEffectNode* effect);
   ~PostProcessMechanism();

   void render();
};

///////////////////////////////////////////////////////////////////////////////
