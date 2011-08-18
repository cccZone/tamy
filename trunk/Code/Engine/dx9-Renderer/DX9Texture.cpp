#include "core-Renderer\Texture.h"
#include "dx9-Renderer\DX9Renderer.h"


///////////////////////////////////////////////////////////////////////////////

void RCCreateTexture::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   IDirect3DTexture9* dxTexture = dxRenderer.getTexture( m_texture );
   if ( dxTexture )
   {
      D3DSURFACE_DESC desc;
      dxTexture->GetLevelDesc( 0, &desc );

      m_width = desc.Width;
      m_height = desc.Height;
   }
}

///////////////////////////////////////////////////////////////////////////////
