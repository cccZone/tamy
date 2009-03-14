#include "D3DMaterial.h"


///////////////////////////////////////////////////////////////////////////////

D3DMaterial::D3DMaterial(Texture& emptyTexture, 
                         unsigned int index, 
                         IDirect3DDevice9& d3Device)
      : Material(emptyTexture, index),
      m_d3Device(d3Device)
{
}

///////////////////////////////////////////////////////////////////////////////

void D3DMaterial::enableTransparency(bool enable)
{
   if (enable)
   {
      m_d3Device.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
      m_d3Device.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
      m_d3Device.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

      m_d3Device.SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
      m_d3Device.SetRenderState(D3DRS_ALPHAREF, (DWORD)0x0000000E);
      m_d3Device.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

      m_d3Device.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
      m_d3Device.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
      m_d3Device.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_ADDSIGNED);

      m_d3Device.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
   }
   else
   {
      m_d3Device.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
      m_d3Device.SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
      m_d3Device.SetRenderState(D3DRS_ZWRITEENABLE, TRUE); 

      m_d3Device.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
      m_d3Device.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
   }
}

///////////////////////////////////////////////////////////////////////////////
