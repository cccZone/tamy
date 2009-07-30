#include "core-Renderer\PostProcessMechanism.h"
#include "core-Renderer\PostProcessEffectNode.h"
#include "core-Renderer\RenderingTechnique.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

PostProcessMechanism::PostProcessMechanism(RenderingTargetsPolicy* policy,
                                           RenderingTechnique& technique,
                                           PostProcessEffectNode* effect)
      : RenderingMechanism(policy),
      m_technique(technique),
      m_effect(effect)
{
   if (effect == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a PostProcessEffectNode instance");
   }

   m_renderables.push_back(effect);
}

///////////////////////////////////////////////////////////////////////////////

PostProcessMechanism::~PostProcessMechanism()
{
   m_renderables.clear();

   delete m_effect;
   m_effect = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessMechanism::render()
{   
   m_technique.render(m_renderables);
}

///////////////////////////////////////////////////////////////////////////////
