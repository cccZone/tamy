#include "dx9-Renderer\DX9RenderTarget.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "dx9-Renderer\DXErrorParser.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

DX9RenderTarget::DX9RenderTarget( RenderTarget& renderTarget )
: m_renderTarget( renderTarget )
, m_renderer(NULL)
, m_dxTexture( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

DX9RenderTarget::~DX9RenderTarget()
{
   if (m_renderer != NULL)
   {
      dynamic_cast< Subject< DX9Renderer, DX9GraphResourceOp >* >( m_renderer )->detachObserver( *this );
      m_renderer = NULL;
   }

   if ( m_dxTexture != NULL )
   {
      m_dxTexture->Release();
      m_dxTexture = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderTarget::initialize( Renderer& renderer )
{
   m_renderer = dynamic_cast< DX9Renderer* >( &renderer );
   if (m_renderer == NULL)
   {
      throw std::runtime_error("This implementation can work only with DX9Renderer");
   }

   createTexture( *m_renderer );

   // attach the instance as the renderer observer
   dynamic_cast< Subject< DX9Renderer, DX9GraphResourceOp >* >( m_renderer )->attachObserver( *this );
}

///////////////////////////////////////////////////////////////////////////////

void* DX9RenderTarget::getPlatformSpecific() const
{
   return m_dxTexture;
}

///////////////////////////////////////////////////////////////////////////////

// TODO: do robienia snapshot'ow, albo tekstur - opakowac  IDirect3DSurface9 w jakas
// klase (w stylu Bitmap albo cos), tam wpakowac metody w stylu getPixel itd,
// a tu tylko zostawic metode ktora tworzy taka bitmape (grabSnapshot)

Color DX9RenderTarget::getPixel( const D3DXVECTOR2& pos ) const
{
   if ( !m_dxTexture )
   {
      return Color();
   }

   IDirect3DDevice9& d3Dev = m_renderer->getD3Device();
   D3DSURFACE_DESC rtDesc;
   m_dxTexture->GetLevelDesc( 0, &rtDesc );

   // create a surface in system memory to which we'll copy the snapshot of the render target's contents
   IDirect3DSurface9* offscreenSurface = NULL;
   HRESULT res = d3Dev.CreateOffscreenPlainSurface( 
      rtDesc.Width, rtDesc.Height, 
      rtDesc.Format, 
      D3DPOOL_SYSTEMMEM, 
      &offscreenSurface, 
      NULL );
   if( FAILED( res ) )
   {
      std::string errMsg = translateDxError( "Can't create an off-screen plain surface", res );
      throw std::runtime_error( errMsg );
   }

   // copy the data from the render target to the temporary surface
   IDirect3DSurface9* rtSurface = NULL;
   m_dxTexture->GetSurfaceLevel( 0, &rtSurface );
   res = d3Dev.GetRenderTargetData( rtSurface, offscreenSurface );
   if( FAILED( res ) )
   {
      std::string errMsg = translateDxError( "Can't acquire data from a render target", res );
      throw std::runtime_error( errMsg );
   }


   // lookup the specified pixel and return its color
   Color color;
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
      throw std::runtime_error( errMsg );
   }

   unsigned int x = ( unsigned int )( ( rtDesc.Width * ( pos.x + 1.f ) ) / 2.f );
   unsigned int y = ( unsigned int )( ( rtDesc.Height * ( 1.f - pos.y ) ) / 2.f );

   unsigned int addr = y * lockedRect.Pitch + x * 4;
   unsigned char* ptr = ( unsigned char* )( lockedRect.pBits ) + addr;
   color.b = ptr[0] / 255.f;
   color.g = ptr[1] / 255.f;
   color.r = ptr[2] / 255.f;
   color.a = ptr[3] / 255.f;

   offscreenSurface->UnlockRect();

   // cleanup
   rtSurface->Release();
   offscreenSurface->Release();

   return color;
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderTarget::update( DX9Renderer& renderer )
{
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderTarget::update( DX9Renderer& renderer, const DX9GraphResourceOp& operation )
{
   switch(operation)
   {
   case GRO_RELEASE_RES:
      {
         if ( m_dxTexture )
         {
            m_dxTexture->Release();
            m_dxTexture = NULL;
         }
         break;
      }

   case GRO_CREATE_RES:
      {
         createTexture( renderer );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderTarget::createTexture( DX9Renderer& renderer )
{
   // create the texture
   D3DFORMAT texFormat = renderer.getOptimalTextureFormat( m_renderTarget.getUsage() );
   unsigned int width = m_renderTarget.getWidth();
   unsigned int height = m_renderTarget.getHeight();

   HRESULT res = D3DXCreateTexture( &renderer.getD3Device(),
      width, height,          // dimensions
      1,                      // MIP levels
      D3DUSAGE_RENDERTARGET,  // usage
      texFormat,              // format
      D3DPOOL_DEFAULT,        // memory pool
      &m_dxTexture);

   if ( FAILED(res) || m_dxTexture == NULL )
   {
      std::string errorMsg = translateDxError( "Can't create a render target", res );
      throw std::runtime_error( errorMsg );
   }
}

///////////////////////////////////////////////////////////////////////////////
