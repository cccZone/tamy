#include "impl-DirectX\D3DRenderingTargetsCleaner.h"


///////////////////////////////////////////////////////////////////////////////

D3DRenderingTargetsCleaner::D3DRenderingTargetsCleaner(IDirect3DDevice9& d3Device)
: m_d3Device(d3Device)
{
}

///////////////////////////////////////////////////////////////////////////////

void D3DRenderingTargetsCleaner::cleanRenderingTargets(unsigned int targetsCount)
{
   for (unsigned int i = 0; i < targetsCount; ++i)
   {
      m_d3Device.Clear(i, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x0032C1FF, 1.0f, 0);
   }
}

///////////////////////////////////////////////////////////////////////////////
