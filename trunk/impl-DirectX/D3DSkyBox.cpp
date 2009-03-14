#include "D3DSkyBox.h"
#include <stdexcept>
#include <string>


///////////////////////////////////////////////////////////////////////////////

DWORD D3DSkyBoxVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

///////////////////////////////////////////////////////////////////////////////

D3DSkyBox::D3DSkyBox(IDirect3DDevice9& d3Device, IDirect3DVertexBuffer9* vb)
      : m_d3Device(d3Device),
      m_vb(vb)
{
   m_d3Device.AddRef();
   m_vb->AddRef();

   D3DXMatrixIdentity(&m_cameraWorldMtx);
}

///////////////////////////////////////////////////////////////////////////////

D3DSkyBox::~D3DSkyBox()
{
   m_vb->Release();
   m_vb = NULL;

   m_d3Device.Release();
}

///////////////////////////////////////////////////////////////////////////////

void D3DSkyBox::updateOrientation(const D3DXMATRIX& cameraWorldMtx) 
{
   m_cameraWorldMtx._41 = cameraWorldMtx._41;
   m_cameraWorldMtx._42 = cameraWorldMtx._42;
   m_cameraWorldMtx._43 = cameraWorldMtx._43;
}

///////////////////////////////////////////////////////////////////////////////

void D3DSkyBox::startRendering()
{
   m_d3Device.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

   m_d3Device.SetFVF(D3DSkyBoxVertex::FVF);
   m_d3Device.SetTransform(D3DTS_WORLD, &m_cameraWorldMtx);
   m_d3Device.SetStreamSource(0, m_vb, 0, sizeof(D3DSkyBoxVertex));
}

///////////////////////////////////////////////////////////////////////////////

void D3DSkyBox::renderSide(SkyBoxSides sideIdx)
{
   m_d3Device.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
   m_d3Device.SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
   m_d3Device.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
   m_d3Device.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
   m_d3Device.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
   m_d3Device.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
   m_d3Device.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
   m_d3Device.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
   m_d3Device.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

   m_d3Device.DrawPrimitive(D3DPT_TRIANGLESTRIP, sideIdx * 4, 2);
}

///////////////////////////////////////////////////////////////////////////////

void D3DSkyBox::endRendering()
{
   m_d3Device.SetTexture(0, NULL);

   m_d3Device.SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
