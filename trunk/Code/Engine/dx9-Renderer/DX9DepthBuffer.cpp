#include "dx9-Renderer\DX9DepthBuffer.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "dx9-Renderer\DXErrorParser.h"
#include "core-Renderer\DepthBuffer.h"


///////////////////////////////////////////////////////////////////////////////

DX9DepthBuffer::DX9DepthBuffer( const DX9Renderer& renderer, const DepthBuffer& depthBuffer )
   : m_renderer( renderer )
   , m_depthBuffer( depthBuffer )
   , m_depthSurface( NULL )
   , m_isActive( false )
{
   createDepthBuffer();
}

///////////////////////////////////////////////////////////////////////////////

DX9DepthBuffer::~DX9DepthBuffer()
{
   if ( m_depthSurface )
   {
      m_depthSurface->Release();
      m_depthSurface = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9DepthBuffer::activate()
{
   ASSERT_MSG( !m_isActive, "This buffer is already active" );
   m_isActive = true;

   // activate the new surface
   bind();
}

///////////////////////////////////////////////////////////////////////////////

void DX9DepthBuffer::deactivate()
{
   ASSERT_MSG( m_isActive, "This buffer is not active" );
   m_isActive = false;

   IDirect3DDevice9& d3Device = m_renderer.getD3Device();
   d3Device.SetDepthStencilSurface( NULL );
}

///////////////////////////////////////////////////////////////////////////////

void DX9DepthBuffer::onLostDevice()
{
   if ( m_depthSurface )
   {
      m_depthSurface->Release();
      m_depthSurface = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9DepthBuffer::onResetDevice()
{
   createDepthBuffer();

   if ( m_isActive )
   {
      // the buffer is an active buffer, so bind it to the device
      bind();
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9DepthBuffer::createDepthBuffer()
{
   D3DFORMAT format = m_renderer.getOptimalTextureFormat( TU_DEPTH_STENCIL );
   uint width, height;
   m_depthBuffer.getDimensions( width, height );

   IDirect3DDevice9& d3Device = m_renderer.getD3Device();

   HRESULT res = d3Device.CreateDepthStencilSurface( 
      width, height,          // dimensions
      format,                 // format
      D3DMULTISAMPLE_NONE,    // multi sampling type
      0,                      // multi sampling quality
      TRUE,                   // should the depth buffer be automatically discarded when this one becomes in use
      &m_depthSurface,
      NULL );

   if ( FAILED(res) || m_depthSurface == NULL )
   {
      std::string errMsg = translateDxError( "Can't create a depth buffer surface", res );
      ASSERT_MSG( false, errMsg.c_str() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9DepthBuffer::bind()
{
   IDirect3DDevice9& d3Device = m_renderer.getD3Device();
   HRESULT res = d3Device.SetDepthStencilSurface( m_depthSurface );
   if ( FAILED(res) )
   {
      std::string errMsg = translateDxError( "Unable to activate a depth buffer", res );
      ASSERT_MSG( false, errMsg.c_str() );
   }
}

///////////////////////////////////////////////////////////////////////////////
