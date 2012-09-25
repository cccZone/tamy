#include "core-Renderer/FullscreenQuad.h"
#include "dx9-Renderer/DX9Renderer.h"
#include "dx9-Renderer/DXErrorParser.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

struct QuadVertex
{
   D3DXVECTOR4    pos;
   D3DXVECTOR2    texCoords;
};

///////////////////////////////////////////////////////////////////////////////

void RCFullscreenQuad::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   // To correctly map from texels->pixels we offset the coordinates
   // by -0.5f:
   float fWidth = (float)m_width + 0.5f;
   float fHeight = (float)m_height + 0.5f;

   // Now we can actually assemble the screen-space geometry
   static QuadVertex v[4];

   v[0].pos = D3DXVECTOR4( -0.5f, -0.5f, 0.0f, 1.0f );
   v[0].texCoords = D3DXVECTOR2( 0.0f, 0.0f );

   v[1].pos = D3DXVECTOR4( fWidth, -0.5f, 0.0f, 1.0f );
   v[1].texCoords = D3DXVECTOR2( 1.0f, 0.0f );

   v[2].pos = D3DXVECTOR4( -0.5f, fHeight, 0.0f, 1.0f );
   v[2].texCoords = D3DXVECTOR2( 0.0f, 1.0f );

   v[3].pos = D3DXVECTOR4( fWidth, fHeight, 0.0f, 1.0f );
   v[3].texCoords = D3DXVECTOR2( 1.0f, 1.0f );

   // Configure the device and render..
   d3Device.SetVertexShader( NULL );
   d3Device.SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
   d3Device.DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof( QuadVertex ) );
}

///////////////////////////////////////////////////////////////////////////////

void RCScreenQuad::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   // To correctly map from texels->pixels we offset the coordinates
   // by -0.5f:
   float fOffsetX = (float)m_offsetX;
   float fOffsetY = (float)m_offsetY;
   float fWidth = (float)m_width + 0.5f;
   float fHeight = (float)m_height + 0.5f;

   // Now we can actually assemble the screen-space geometry
   static QuadVertex v[4];

   v[0].pos = D3DXVECTOR4( fOffsetX,            fOffsetY,            0.0f, 1.0f );
   v[0].texCoords = D3DXVECTOR2( 0.0f, 0.0f );

   v[1].pos = D3DXVECTOR4( fOffsetX + fWidth,   fOffsetY,            0.0f, 1.0f );
   v[1].texCoords = D3DXVECTOR2( 1.0f, 0.0f );

   v[2].pos = D3DXVECTOR4( fOffsetX,            fOffsetY + fHeight,  0.0f, 1.0f );
   v[2].texCoords = D3DXVECTOR2( 0.0f, 1.0f );

   v[3].pos = D3DXVECTOR4( fOffsetX + fWidth,   fOffsetY + fHeight,  0.0f, 1.0f );
   v[3].texCoords = D3DXVECTOR2( 1.0f, 1.0f );

   // Configure the device and render..
   d3Device.SetVertexShader( NULL );
   d3Device.SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
   d3Device.DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof( QuadVertex ) );
}

///////////////////////////////////////////////////////////////////////////////
