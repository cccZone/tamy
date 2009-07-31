#include "impl-DirectX\D3DSkyBox.h"
#include <stdexcept>
#include <string>


///////////////////////////////////////////////////////////////////////////////

DWORD D3DSkyBoxVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

///////////////////////////////////////////////////////////////////////////////

D3DSkyBox::D3DSkyBox(MaterialImpl* materialImpl, 
                     RenderingTargetsPolicy& policy,
                     IDirect3DDevice9& d3Device, 
                     IDirect3DVertexBuffer9* vb)
      : SkyBox(materialImpl, policy),
      m_d3Device(d3Device),
      m_vb(vb)
{
   D3DXMatrixIdentity(&m_cameraWorldMtx);
}

///////////////////////////////////////////////////////////////////////////////

D3DSkyBox::~D3DSkyBox()
{
   m_vb->Release();
   m_vb = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void D3DSkyBox::startRendering()
{
   m_d3Device.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
   m_d3Device.SetRenderState(D3DRS_ZENABLE, FALSE);

   D3DXMATRIX globalMtx = getGlobalMtx();
   m_cameraWorldMtx._41 = globalMtx._41;
   m_cameraWorldMtx._42 = globalMtx._42;
   m_cameraWorldMtx._43 = globalMtx._43;

   m_d3Device.SetFVF(D3DSkyBoxVertex::FVF);
   m_d3Device.SetTransform(D3DTS_WORLD, &m_cameraWorldMtx);
   m_d3Device.SetStreamSource(0, m_vb, 0, sizeof(D3DSkyBoxVertex));

   m_d3Device.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
   m_d3Device.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
   m_d3Device.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
   m_d3Device.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
   m_d3Device.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

   m_d3Device.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
   m_d3Device.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

   m_d3Device.SetRenderState(D3DRS_LIGHTING, FALSE);
}

///////////////////////////////////////////////////////////////////////////////

void D3DSkyBox::renderSide(SkyBoxSides sideIdx)
{
   m_d3Device.DrawPrimitive(D3DPT_TRIANGLESTRIP, sideIdx * 4, 2);
}

///////////////////////////////////////////////////////////////////////////////

void D3DSkyBox::endRendering()
{
   m_d3Device.SetTexture(0, NULL);
   m_d3Device.SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
   m_d3Device.SetRenderState(D3DRS_ZENABLE, TRUE);
   m_d3Device.SetRenderState(D3DRS_LIGHTING, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
