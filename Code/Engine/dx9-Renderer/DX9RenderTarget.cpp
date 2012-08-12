#include "dx9-Renderer\DX9RenderTarget.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "dx9-Renderer\DXErrorParser.h"
#include "core\Vector.h"
#include "core\Color.h"
#include "core\Assert.h"
#include "core\Log.h"


///////////////////////////////////////////////////////////////////////////////

void RCGetPixel::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );

   DX9RenderTarget* dxRenderTarget = dxRenderer.getRenderTarget( m_renderTarget );
   if ( dxRenderTarget )
   {
      dxRenderTarget->getPixel( m_queryPos, m_outColorVal );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DX9RenderTarget::DX9RenderTarget( const DX9Renderer& renderer, const RenderTarget& renderTarget )
   : m_renderer( renderer )
   , m_renderTarget( renderTarget )
   , m_dxTexture( NULL )
{
   createTexture();
}

///////////////////////////////////////////////////////////////////////////////

DX9RenderTarget::~DX9RenderTarget()
{
   if ( m_dxTexture != NULL )
   {
      m_dxTexture->Release();
      m_dxTexture = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderTarget::getPixel( const Vector& pos, Color& outColor ) const
{
   if ( !m_dxTexture )
   {
      return;
   }

   IDirect3DDevice9& d3Dev = m_renderer.getD3Device();
   D3DSURFACE_DESC rtDesc;
   m_dxTexture->GetLevelDesc( 0, &rtDesc );

   // create a surface in system memory to which we'll copy the snapshot of the render target's contents
   IDirect3DSurface9* offscreenSurface = NULL;
   HRESULT res = d3Dev.CreateOffscreenPlainSurface( rtDesc.Width, rtDesc.Height, rtDesc.Format, D3DPOOL_SYSTEMMEM, &offscreenSurface, NULL );
   if( FAILED( res ) )
   {
      std::string errMsg = translateDxError( "Can't create an off-screen plain surface", res );
      ASSERT_MSG( false, errMsg.c_str() );
      return;
   }

   // check the validity of the queried position
   if ( pos.x >= rtDesc.Width || pos.y >= rtDesc.Height )
   {
      ASSERT_MSG( false, "Queried position is outside the screen boundaries." );
      return;
   }

   // copy the data from the render target to the temporary surface
   IDirect3DSurface9* rtSurface = NULL;
   m_dxTexture->GetSurfaceLevel( 0, &rtSurface );
   res = d3Dev.GetRenderTargetData( rtSurface, offscreenSurface );
   if( FAILED( res ) )
   {
      std::string errMsg = translateDxError( "Can't acquire data from a render target", res );
      ASSERT_MSG( false, errMsg.c_str() );
      return;
   }


   // lookup the specified pixel and return its color
   D3DLOCKED_RECT lockedRect;
   RECT rect;
   rect.left = 0;
   rect.right = rtDesc.Width;
   rect.top = 0;
   rect.bottom = rtDesc.Height;

   res = offscreenSurface->LockRect( &lockedRect, &rect, D3DLOCK_READONLY );
   if( FAILED( res ) )
   {
      std::string errMsg = translateDxError( "Can't lock an off-screen plain surface", res );
      ASSERT_MSG( false, errMsg.c_str() );
      return;
   }

   unsigned int x = ( unsigned int )( ( rtDesc.Width * ( pos.x + 1.f ) ) / 2.f );
   unsigned int y = ( unsigned int )( ( rtDesc.Height * ( 1.f - pos.y ) ) / 2.f );

   unsigned int addr = y * lockedRect.Pitch + x * 4;
   unsigned char* ptr = ( unsigned char* )( lockedRect.pBits ) + addr;
   outColor.b = ptr[0] / 255.f;
   outColor.g = ptr[1] / 255.f;
   outColor.r = ptr[2] / 255.f;
   outColor.a = ptr[3] / 255.f;

   offscreenSurface->UnlockRect();

   // cleanup
   rtSurface->Release();
   offscreenSurface->Release();
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderTarget::onLostDevice()
{
   if ( m_dxTexture )
   {
      m_dxTexture->Release();
      m_dxTexture = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderTarget::onResetDevice()
{
   createTexture();
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderTarget::createTexture()
{
   // create the texture
   D3DFORMAT texFormat = m_renderer.getOptimalTextureFormat( m_renderTarget.getUsage() );
   unsigned int width = m_renderTarget.getWidth();
   unsigned int height = m_renderTarget.getHeight();

   IDirect3DDevice9& d3Device = m_renderer.getD3Device();

   // make sure the render target dimensions are not smaller the depth buffer dimensions, otherwise
   // nothing will get rendered to this render target
   {
      IDirect3DSurface9 *pZStencilSurface;
      d3Device.GetDepthStencilSurface( &pZStencilSurface );

      D3DSURFACE_DESC surfDesc;
      pZStencilSurface->GetDesc( &surfDesc );

      // adjust render target's dimensions, and inform the user about it
      CONDITIONAL_LOG( width <= surfDesc.Width && height <= surfDesc.Height, "This render target is larger than depth buffer dimensions. It's dimensions will be reduced to fit those of thye depth buffer." );
      width = min( width, surfDesc.Width );
      height = min( height, surfDesc.Height );

      // release the pointer to the surface - we no longer need it
      pZStencilSurface->Release();
   }

   HRESULT res = D3DXCreateTexture( &d3Device,
      width, height,          // dimensions
      1,                      // MIP levels
      D3DUSAGE_RENDERTARGET,  // usage
      texFormat,              // format
      D3DPOOL_DEFAULT,        // memory pool
      &m_dxTexture);


   if ( FAILED(res) || m_dxTexture == NULL )
   {
      std::string errMsg = translateDxError( "Can't create a render target", res );
      ASSERT_MSG( false, errMsg.c_str() );
   }
}

///////////////////////////////////////////////////////////////////////////////
