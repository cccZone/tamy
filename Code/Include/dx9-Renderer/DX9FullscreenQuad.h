#pragma once

/// @file   dx9-Renderer\DX9FullscreenQuad.h
/// @brief  dx9 implementation of a full-screen quad geometry

#include "core-Renderer\FullscreenQuad.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

class DX9FullscreenQuad : public FullscreenQuadImpl
{
private:
   struct QuadVertex
   {
      D3DXVECTOR4    pos;
      D3DXVECTOR2    texCoords;
   };

private:
   FullscreenQuad&               m_quad;
   IDirect3DDevice9*             m_d3Device;

public:
   DX9FullscreenQuad( FullscreenQuad& quad );
   ~DX9FullscreenQuad();

   // -------------------------------------------------------------------------
   // FullscreenQuadImpl implementation
   // -------------------------------------------------------------------------
   void render( unsigned int width, unsigned int height );
   void initialize( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////


