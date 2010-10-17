#include "dx9-Renderer/DX9FullscreenQuad.h"
#include "dx9-Renderer/DX9Renderer.h"
#include "dx9-Renderer/DXErrorParser.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

DX9FullscreenQuad::DX9FullscreenQuad( FullscreenQuad& quad )
   : m_quad( quad )
   , m_d3Device( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

DX9FullscreenQuad::~DX9FullscreenQuad()
{
   m_d3Device = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DX9FullscreenQuad::initialize( Renderer& renderer )
{
   DX9Renderer* d3dRenderer = dynamic_cast< DX9Renderer* >( &renderer );
   ASSERT_MSG( d3dRenderer != NULL, "This class works only with DX9Renderer instance" );
   if ( d3dRenderer == NULL )
   {
      return;
   }

   m_d3Device = &d3dRenderer->getD3Device();
}

///////////////////////////////////////////////////////////////////////////////

void DX9FullscreenQuad::render()
{
   if ( m_d3Device )
   {
      // To correctly map from texels->pixels we offset the coordinates
      // by -0.5f:
      float fWidth = static_cast< float >( m_quad.getWidth() ) - 0.5f;
      float fHeight = static_cast< float >( m_quad.getHeight() ) - 0.5f;

      // Now we can actually assemble the screen-space geometry
      QuadVertex v[4];

      v[0].pos = D3DXVECTOR4( -0.5f, -0.5f, 0.0f, 1.0f );
      v[0].texCoords = D3DXVECTOR2( 0.0f, 0.0f );

      v[1].pos = D3DXVECTOR4( fWidth, -0.5f, 0.0f, 1.0f );
      v[1].texCoords = D3DXVECTOR2( 1.0f, 0.0f );

      v[2].pos = D3DXVECTOR4( -0.5f, fHeight, 0.0f, 1.0f );
      v[2].texCoords = D3DXVECTOR2( 0.0f, 1.0f );

      v[3].pos = D3DXVECTOR4( fWidth, fHeight, 0.0f, 1.0f );
      v[3].texCoords = D3DXVECTOR2( 1.0f, 1.0f );

      // Configure the device and render..
      m_d3Device->SetVertexShader( NULL );
      m_d3Device->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
      m_d3Device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof( QuadVertex ) );
   }
}

///////////////////////////////////////////////////////////////////////////////
