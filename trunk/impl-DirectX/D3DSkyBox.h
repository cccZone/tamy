#pragma once

#include "SkyBox.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * A DirectX representation of a skybox
 */
class D3DSkyBox : public SkyBox
{
private:
   IDirect3DDevice9& m_d3Device;
   IDirect3DVertexBuffer9* m_vb;
   D3DXMATRIX m_cameraWorldMtx;

public:
   D3DSkyBox(IDirect3DDevice9& d3Device, IDirect3DVertexBuffer9* vb);
   ~D3DSkyBox();

   void updateOrientation(const D3DXMATRIX& cameraWorldMtx);

protected:
   void startRendering();
   void renderSide(SkyBoxSides sideIdx);
   void endRendering();
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
