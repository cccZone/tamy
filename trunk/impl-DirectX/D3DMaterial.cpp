#include "impl-DirectX\D3DMaterial.h"


///////////////////////////////////////////////////////////////////////////////

D3DMaterial::D3DMaterial(IDirect3DDevice9& d3Device, 
                         LightReflectingProperties& lrp, 
                         MaterialOperationImplementation& alphaMatOp,
                         MaterialOperationImplementation& colorMatOp,
                         unsigned int index)
      : Material(lrp, alphaMatOp, colorMatOp, index),
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

      m_d3Device.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
   }
   else
   {
      m_d3Device.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
      m_d3Device.SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
      m_d3Device.SetRenderState(D3DRS_ZWRITEENABLE, TRUE); 
   }
}

///////////////////////////////////////////////////////////////////////////////
