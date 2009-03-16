#include "impl-DirectX\D3DTexture.h"


///////////////////////////////////////////////////////////////////////////////

D3DTexture::D3DTexture(const std::string& name, 
                       IDirect3DDevice9& d3Device, 
                       IDirect3DTexture9& texture)
      : Texture(name),
      m_d3Device(d3Device),
      m_texture(&texture)
{
   m_d3Device.AddRef();
   m_texture->AddRef();
}

///////////////////////////////////////////////////////////////////////////////

D3DTexture::~D3DTexture()
{
   m_d3Device.Release();
   m_texture->Release();
}

///////////////////////////////////////////////////////////////////////////////

void D3DTexture::setForRendering()
{
   m_d3Device.SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
   m_d3Device.SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
   m_d3Device.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
   m_d3Device.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
   m_d3Device.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

   m_d3Device.SetTexture(0, m_texture);
}

///////////////////////////////////////////////////////////////////////////////
