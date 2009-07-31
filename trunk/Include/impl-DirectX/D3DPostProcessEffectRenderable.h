#pragma once

#include "core-Renderer\PostProcessEffectRenderable.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DRenderer;

///////////////////////////////////////////////////////////////////////////////

class D3DPostProcessEffectRenderable : public PostProcessEffectRenderable
{
private:
   struct ScreenVertex
   {
      D3DXVECTOR4 coords;
      D3DXVECTOR2 texCoords;

      ScreenVertex(const D3DXVECTOR4& _coords,
                   const D3DXVECTOR2& _texCoords)
            : coords(_coords),
            texCoords(_texCoords)
      {}

      static DWORD FVF;
   };

private:
   IDirect3DDevice9& m_d3Device;
   D3DRenderer& m_renderer;
   IDirect3DVertexBuffer9* m_vb;

public:
   D3DPostProcessEffectRenderable(IDirect3DDevice9& d3Device,
                            D3DRenderer& renderer);
   ~D3DPostProcessEffectRenderable();

   void render();

protected:
   void recreateGeometry(unsigned int width, unsigned int height);

private:
   void releaseQuad();
};

///////////////////////////////////////////////////////////////////////////////