#include "dx9-Renderer\DX9TextureResourcesStorage.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "dx9-Renderer\DXErrorParser.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template<>
IDirect3DTexture9* RenderResourceStorage< DX9Renderer, Texture, IDirect3DTexture9 >::createResource( const Texture& obj ) const
{
   // create the texture
   D3DFORMAT texFormat = m_renderer.getOptimalTextureFormat( obj.getUsage() );
   HRESULT res;

   byte* imageBuffer = NULL;
   unsigned int bufferSize = 0;
   obj.getBuffer( imageBuffer, bufferSize );

   IDirect3DTexture9* dxTexture = NULL;
   IDirect3DDevice9* d3Device = &m_renderer.getD3Device();
   res = D3DXCreateTextureFromFileInMemoryEx( d3Device,
      imageBuffer, bufferSize,
      D3DX_DEFAULT, D3DX_DEFAULT,
      D3DX_DEFAULT, 0, texFormat,
      D3DPOOL_MANAGED, 
      D3DX_DEFAULT, D3DX_DEFAULT,
      D3DCOLOR(), NULL, NULL, &dxTexture );

   if ( FAILED( res ) )
   {
      std::string errorMsg = translateDxError( "Can't load texture " + obj.getTextureName(), res );
      throw std::runtime_error( errorMsg );
   }

   return dxTexture;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, Texture, IDirect3DTexture9 >::releaseResource( IDirect3DTexture9* resource ) const
{
   if( resource )
   {
      resource->Release();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, Texture, IDirect3DTexture9 >::deviceLost( IDirect3DTexture9* resource ) const
{
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, Texture, IDirect3DTexture9 >::deviceRestored( IDirect3DTexture9* resource ) const
{
}

///////////////////////////////////////////////////////////////////////////////
