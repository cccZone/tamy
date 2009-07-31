#include "core-Renderer\PostProcessEffectRenderable.h"
#include "core-Renderer\Renderer.h"


///////////////////////////////////////////////////////////////////////////////

PostProcessEffectRenderable::PostProcessEffectRenderable(Renderer& renderer)
      : m_renderer(renderer)
{
   m_renderer.attachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

PostProcessEffectRenderable::~PostProcessEffectRenderable()
{
   m_renderer.detachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessEffectRenderable::update(Renderer& renderer)
{
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessEffectRenderable::update(Renderer& renderer, const RendererOps& operation)
{
   if (operation == RO_RESIZE_VIEWPORT)
   {
      recreateGeometry(renderer.getViewportWidth(), renderer.getViewportHeight());
   }
}

///////////////////////////////////////////////////////////////////////////////
