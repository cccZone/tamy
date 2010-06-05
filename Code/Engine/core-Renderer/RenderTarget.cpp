#include "core-Renderer\RenderTarget.h"
#include "core-Renderer\Renderer.h"
#include <stdexcept>


BEGIN_RTTI( RenderTarget )
END_RTTI
///////////////////////////////////////////////////////////////////////////////

RenderTarget::RenderTarget( Renderer* renderer, bool isReadable, const Color& bgColor )
: m_bgColor( bgColor )
, m_isReadable( isReadable )
, m_width( 0 )
, m_height( 0 )
, m_renderer( renderer )
{
   if ( m_renderer )
   {
      m_renderer->attachObserver( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderTarget::~RenderTarget()
{
   if ( m_renderer )
   {
      m_renderer->detachObserver( *this );
      m_renderer = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void* RenderTarget::getPlatformSpecific() const
{
   return impl().getPlatformSpecific();
}

///////////////////////////////////////////////////////////////////////////////

Color RenderTarget::getPixel( const D3DXVECTOR2& pos ) const
{
   if ( !m_isReadable )
   {
      throw std::logic_error( "This render target does not support reading operations" );
   }

   return impl().getPixel( pos );
}

///////////////////////////////////////////////////////////////////////////////

void RenderTarget::update( Renderer& renderer )
{
   m_width = renderer.getViewportWidth();
   m_height = renderer.getViewportHeight();
   renderer.implement< RenderTarget >( *this );
}

///////////////////////////////////////////////////////////////////////////////

void RenderTarget::update( Renderer& renderer, const RendererOps& operation )
{
   if ( operation == RO_RESIZE_VIEWPORT )
   {
      m_width = renderer.getViewportWidth();
      m_height = renderer.getViewportHeight();
      renderer.implement< RenderTarget >( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////
