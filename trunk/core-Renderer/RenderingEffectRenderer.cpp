#include "core-Renderer\RenderingEffectRenderer.h"
#include "core-Renderer\GraphicalEffect.h"
#include "core-Renderer\Renderable.h"


///////////////////////////////////////////////////////////////////////////////

RenderingEffectRenderer::RenderingEffectRenderer(GraphicalEffect* effect) 
      : m_effect(effect) 
{
   if (m_effect == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a GraphicalEffect instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderingEffectRenderer::~RenderingEffectRenderer() 
{
   delete m_effect;
   m_effect = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingEffectRenderer::render(Renderable& renderable, RenderingTargetsPolicy& policy)
{
   onRender();
   m_effect->render(renderable, policy);
}

///////////////////////////////////////////////////////////////////////////////
