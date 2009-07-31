#include "core-Renderer\PostProcessMechanism.h"
#include "core-Renderer\PostProcessEffectRenderable.h"
#include "core-Renderer\GraphicalEffect.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

PostProcessMechanism::PostProcessMechanism(RenderingTargetsPolicy* policy,
                                           GraphicalEffect& effect,
                                           PostProcessEffectRenderable* renderable)
      : RenderingMechanism(policy),
      m_effect(effect),
      m_renderable(renderable)
{
   if (renderable == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a PostProcessEffectRenderable instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

PostProcessMechanism::~PostProcessMechanism()
{
   delete m_renderable;
   m_renderable = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessMechanism::render()
{   
   m_effect.render(*m_renderable);
}

///////////////////////////////////////////////////////////////////////////////
