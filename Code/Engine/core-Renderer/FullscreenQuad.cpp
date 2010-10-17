#include "core-Renderer\FullscreenQuad.h"
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\Renderer.h"
#include "core\ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RTTI( FullscreenQuad )
END_RTTI

///////////////////////////////////////////////////////////////////////////////

FullscreenQuad::FullscreenQuad( Renderer* renderer )
   : m_renderer( renderer )
{
   if ( m_renderer )
   {
      m_renderer->attachObserver( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

FullscreenQuad::~FullscreenQuad()
{
   if ( m_renderer )
   {
      m_renderer->detachObserver( *this );
      m_renderer = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void FullscreenQuad::initialize( ResourcesManager& rm )
{
}

///////////////////////////////////////////////////////////////////////////////

void FullscreenQuad::update( Renderer& renderer )
{
   m_width = renderer.getViewportWidth();
   m_height = renderer.getViewportHeight();
   renderer.implement< FullscreenQuad >( *this );
}

///////////////////////////////////////////////////////////////////////////////

void FullscreenQuad::update( Renderer& renderer, const RendererOps& operation )
{
   if ( operation == RO_RESIZE_VIEWPORT )
   {
      m_width = renderer.getViewportWidth();
      m_height = renderer.getViewportHeight();
      renderer.implement< FullscreenQuad >( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////


void FullscreenQuad::render()
{
   impl().render();
}

///////////////////////////////////////////////////////////////////////////////
