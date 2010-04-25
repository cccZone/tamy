#include "impl-DirectX\D3DSkyBoxSide.h"
#include <stdexcept>
#include <string>


///////////////////////////////////////////////////////////////////////////////

DWORD D3DSkyBoxVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

///////////////////////////////////////////////////////////////////////////////

D3DSkyBoxSide::D3DSkyBoxSide(Texture* texture,
                             IDirect3DDevice9& d3Device, 
                             IDirect3DVertexBuffer9* vb)
      : SkyBoxSide(texture),
      m_d3Device(d3Device),
      m_vb(vb)
{
}

///////////////////////////////////////////////////////////////////////////////

D3DSkyBoxSide::~D3DSkyBoxSide()
{
   m_vb->Release();
   m_vb = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void D3DSkyBoxSide::render()
{
   // set the renderer state
   m_d3Device.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
   m_d3Device.SetRenderState(D3DRS_ZENABLE, FALSE);

   m_d3Device.SetFVF(D3DSkyBoxVertex::FVF);
   m_d3Device.SetStreamSource(0, m_vb, 0, sizeof(D3DSkyBoxVertex));

   m_d3Device.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
   m_d3Device.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
   m_d3Device.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
   m_d3Device.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
   m_d3Device.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

   m_d3Device.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
   m_d3Device.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

   m_d3Device.SetRenderState(D3DRS_LIGHTING, FALSE);

   // renderer
   m_d3Device.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

   // rollback the renderer state changes
   m_d3Device.SetTexture(0, NULL);
   m_d3Device.SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
   m_d3Device.SetRenderState(D3DRS_ZENABLE, TRUE);
   m_d3Device.SetRenderState(D3DRS_LIGHTING, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
