#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/Renderer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RenderTargetDescriptor, Object )
   PROPERTY( std::string, m_id )
   PROPERTY( bool, m_isDynamic )
   PROPERTY( unsigned int, m_initialWidth )
   PROPERTY( unsigned int, m_initialHeight )
   PROPERTY( float, m_widthScale )
   PROPERTY( float, m_heightScale )
   PROPERTY( TextureUsage, m_usage )
   PROPERTY( bool, m_isReadable )
   PROPERTY( Color, m_bgColor )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RenderTargetDescriptor::RenderTargetDescriptor()
   : m_initialWidth( 256 )
   , m_initialHeight( 256 )
   , m_widthScale( 1.0f )
   , m_heightScale( 1.0f )
   , m_isDynamic( false )
   , m_renderTarget( NULL )
   , m_usage( TU_COLOR )
   , m_isReadable( false )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderTargetDescriptor::~RenderTargetDescriptor()
{
   delete m_renderTarget;
   m_renderTarget = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetDescriptor::initialize( Renderer& renderer )
{
   delete m_renderTarget;

   if ( m_isDynamic )
   {
      m_renderTarget = new RenderTarget( new RTSPDynamic( renderer, m_widthScale, m_heightScale ), m_usage, m_isReadable, m_bgColor );
   }
   else
   {
      m_renderTarget = new RenderTarget( new RTSPStatic( m_initialWidth, m_initialHeight ), m_usage, m_isReadable, m_bgColor );
   }

   renderer.implement< RenderTarget >( *m_renderTarget );
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetDescriptor::deinitialize()
{
   delete m_renderTarget;
   m_renderTarget = NULL;
}

///////////////////////////////////////////////////////////////////////////////
