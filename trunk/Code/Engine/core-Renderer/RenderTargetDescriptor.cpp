#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/Renderer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RenderTargetDescriptor );
   PROPERTY( std::string, m_id );
   PROPERTY( bool, m_isDynamic );
   PROPERTY( unsigned int, m_initialWidth );
   PROPERTY( unsigned int, m_initialHeight );
   PROPERTY( float, m_widthScale );
   PROPERTY( float, m_heightScale );
   PROPERTY( TextureUsage, m_usage );
   PROPERTY( bool, m_isReadable );
   PROPERTY( Color, m_bgColor );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RenderTargetDescriptor::RenderTargetDescriptor()
   : m_initialWidth( 256 )
   , m_initialHeight( 256 )
   , m_widthScale( 1.0f )
   , m_heightScale( 1.0f )
   , m_isDynamic( false )
   , m_usage( TU_COLOR )
   , m_isReadable( false )
   , m_bgColor( 0, 0, 0, 1 )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderTargetDescriptor::~RenderTargetDescriptor()
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetDescriptor::initialize( RuntimeDataBuffer& runtimeData, Renderer& renderer ) const
{
   runtimeData.registerVar< RenderTarget* >( m_renderTarget );
   if ( m_isDynamic )
   {
      runtimeData[m_renderTarget] = new RenderTarget( new RTSPDynamic( renderer, m_widthScale, m_heightScale ), m_usage, m_isReadable, m_bgColor );
   }
   else
   {
      runtimeData[m_renderTarget] = new RenderTarget( new RTSPStatic( m_initialWidth, m_initialHeight ), m_usage, m_isReadable, m_bgColor );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetDescriptor::deinitialize( RuntimeDataBuffer& runtimeData ) const
{
   delete runtimeData[m_renderTarget];
   runtimeData[m_renderTarget] = NULL;
}

///////////////////////////////////////////////////////////////////////////////
