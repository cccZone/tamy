#include "impl-DirectX\D3DTransparencyEnabler.h"


///////////////////////////////////////////////////////////////////////////////

D3DTransparencyEnabler::D3DTransparencyEnabler(IDirect3DDevice9& d3Device)
      : m_d3Device(d3Device)
{
}

///////////////////////////////////////////////////////////////////////////////

void D3DTransparencyEnabler::setTransparency(bool enable)
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
