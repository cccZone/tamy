#include "core-Renderer\RenderTarget.h"
#include "core-Renderer\Renderer.h"
#include <stdexcept>


BEGIN_RTTI( RenderTarget )
END_RTTI

///////////////////////////////////////////////////////////////////////////////

RenderTarget::RenderTarget( RenderTargetSizePolicy* sizePolicy, TextureUsage usage, bool isReadable, const Color& bgColor )
: m_bgColor( bgColor )
, m_usage( usage )
, m_sizePolicy( sizePolicy )
, m_isReadable( isReadable )
, m_width( 0 )
, m_height( 0 )
{
   if ( m_sizePolicy )
   {
      m_sizePolicy->initialize( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderTarget::~RenderTarget()
{
   delete m_sizePolicy;
   m_sizePolicy = NULL;
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
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RTSPDynamic::RTSPDynamic( Renderer& renderer, float widthScale, float heightScale )
   : m_renderer( renderer )
   , m_widthScale( widthScale )
   , m_heightScale( heightScale )
   , m_hostTarget( NULL )
{
   m_renderer.attachObserver( *this );

   ASSERT_MSG( m_widthScale > 0, "A render target can't have 0-width" );
   if ( m_widthScale <= 0 )
   {
      m_widthScale = 0.1f;
   }

   ASSERT_MSG( m_heightScale > 0, "A render target can't have 0-width" );
   if ( m_heightScale <= 0 )
   {
      m_heightScale = 0.1f;
   }
}

///////////////////////////////////////////////////////////////////////////////

RTSPDynamic::~RTSPDynamic()
{
   m_renderer.detachObserver( *this );
}

///////////////////////////////////////////////////////////////////////////////

void RTSPDynamic::initialize( RenderTarget& target )
{
   m_hostTarget = &target;
   
   unsigned int width = ( unsigned int )( m_renderer.getViewportWidth() * m_widthScale );
   unsigned int height = ( unsigned int )( m_renderer.getViewportHeight() * m_heightScale );

   m_hostTarget->resize( width, height );
   m_renderer.implement< RenderTarget >( *m_hostTarget );
}

///////////////////////////////////////////////////////////////////////////////

void RTSPDynamic::update( Renderer& renderer )
{
   if ( m_hostTarget )
   {
      unsigned int width = ( unsigned int )( renderer.getViewportWidth() * m_widthScale );
      unsigned int height = ( unsigned int )( renderer.getViewportHeight() * m_heightScale );

      m_hostTarget->resize( width, height );
      renderer.implement< RenderTarget >( *m_hostTarget );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RTSPDynamic::update( Renderer& renderer, const RendererOps& operation )
{
   if ( operation == RO_RESIZE_VIEWPORT && m_hostTarget )
   {
      unsigned int width = ( unsigned int )( renderer.getViewportWidth() * m_widthScale );
      unsigned int height = ( unsigned int )( renderer.getViewportHeight() * m_heightScale );

      m_hostTarget->resize( width, height );
      renderer.implement< RenderTarget >( *m_hostTarget );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RTSPStatic::RTSPStatic( unsigned int width, unsigned int height )
   : m_width( width )
   , m_height( height )
{
}

///////////////////////////////////////////////////////////////////////////////

void RTSPStatic::initialize( RenderTarget& target )
{
   target.resize( m_width, m_height );
}

///////////////////////////////////////////////////////////////////////////////
