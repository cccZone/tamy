#pragma once

#include "core-Renderer\SkyBoxSide.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * A DirectX representation of a skybox
 */
class D3DSkyBoxSide : public SkyBoxSide
{
private:
   IDirect3DDevice9& m_d3Device;
   IDirect3DVertexBuffer9* m_vb;

public:
   D3DSkyBoxSide(Texture* texture,
                 IDirect3DDevice9& d3Device, 
                 IDirect3DVertexBuffer9* vb);
   ~D3DSkyBoxSide();

   void render();
};

///////////////////////////////////////////////////////////////////////////////

struct D3DSkyBoxVertex
{
   D3DSkyBoxVertex(float x, float y, float z, float u, float v)
         : m_pos(x, y, z),
         m_u(u), m_v(v)
   {
   }

   D3DXVECTOR3 m_pos;
   float m_u, m_v;

   static DWORD FVF;
};

///////////////////////////////////////////////////////////////////////////////
