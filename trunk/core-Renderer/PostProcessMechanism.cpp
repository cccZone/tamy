#include "core-Renderer\PostProcessMechanism.h"
#include "core-Renderer\PostProcessEffectNode.h"
#include "core-Renderer\RenderingTechnique.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

PostProcessMechanism::PostProcessMechanism(RenderingTargetsPolicy* policy,
                                           PostProcessEffectNode* effect)
      : RenderingMechanism(policy),
      m_effect(effect)
{
   if (effect == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a PostProcessEffectNode instance");
   }

   m_nodesToRender.push_back(effect);
}

///////////////////////////////////////////////////////////////////////////////

PostProcessMechanism::~PostProcessMechanism()
{
   m_nodesToRender.clear();

   delete m_effect;
   m_effect = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessMechanism::render()
{   
   RenderingTechnique& technique = m_effect->getTechnique();
   technique.render(m_nodesToRender, 0, 1);
}

///////////////////////////////////////////////////////////////////////////////
