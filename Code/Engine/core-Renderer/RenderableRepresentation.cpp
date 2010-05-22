#include "core-Renderer\RenderableRepresentation.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\RenderingView.h"
#include "core-Renderer\Effect.h"


///////////////////////////////////////////////////////////////////////////////

RenderableRepresentation::RenderableRepresentation(Renderable& renderable, RenderingView& view) 
: m_renderable(renderable)
, m_view(view)
{}

///////////////////////////////////////////////////////////////////////////////

const Attributed::Attributes& RenderableRepresentation::getAttributes() const
{
   return m_renderable.getAttributes();
}

///////////////////////////////////////////////////////////////////////////////

void RenderableRepresentation::render()
{
   if ( !m_renderable.isVisible() || !m_renderable.hasGeometry() )
   {
      return;
   }

   const Renderable::Effects& effects = m_renderable.getEffects();
   unsigned int count =  effects.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      effects[i]->render(m_renderable.getGeometry());
   }
}

///////////////////////////////////////////////////////////////////////////////
