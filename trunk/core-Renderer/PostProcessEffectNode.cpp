#include "core-Renderer\PostProcessEffectNode.h"
#include "core-Renderer\Renderer.h"


///////////////////////////////////////////////////////////////////////////////

PostProcessEffectNode::PostProcessEffectNode(const std::string& name,
                                     RenderingTechnique& technique,
                                     Renderer& renderer)
      : AbstractGraphicalNode(name, false, technique, 0),
      m_renderer(renderer)
{
   m_renderer.attachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

PostProcessEffectNode::~PostProcessEffectNode()
{
   m_renderer.detachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessEffectNode::update(Renderer& renderer)
{
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessEffectNode::update(Renderer& renderer, const RendererOps& operation)
{
   if (operation == RO_RESIZE_VIEWPORT)
   {
      recreateGeometry(renderer.getViewportWidth(), renderer.getViewportHeight());
   }
}

///////////////////////////////////////////////////////////////////////////////
