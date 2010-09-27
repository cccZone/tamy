#include "core-Renderer\RenderableRepresentation.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\RenderingView.h"


///////////////////////////////////////////////////////////////////////////////

RenderableRepresentation::RenderableRepresentation(Renderable& renderable, RenderingView& view) 
: m_renderable( renderable )
, m_view( view )
{}

///////////////////////////////////////////////////////////////////////////////

const Attributes& RenderableRepresentation::getAttributes() const
{
   return m_renderable.getAttributes();
}

///////////////////////////////////////////////////////////////////////////////

void RenderableRepresentation::render()
{
   // visibility
   m_renderable.render();
}

///////////////////////////////////////////////////////////////////////////////
