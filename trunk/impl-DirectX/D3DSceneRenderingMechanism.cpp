#include "impl-DirectX\D3DSceneRenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

D3DSceneRenderingMechanism::D3DSceneRenderingMechanism(RenderingTargetsPolicy* policy,
                                                       unsigned int maxLightsCount,
                                                       MaterialImpl* impl,
                                                       IDirect3DDevice9& d3Device)
      : SceneRenderingMechanism(policy, maxLightsCount, impl),
      m_d3Device(d3Device)
{
}

///////////////////////////////////////////////////////////////////////////////

void D3DSceneRenderingMechanism::setViewMatrix(const D3DXMATRIX& mtx)
{
   m_d3Device.SetTransform(D3DTS_VIEW, &mtx);
}

/////////////////////////////////////////////////////////////////////////////

void D3DSceneRenderingMechanism::setProjectionMatrix(const D3DXMATRIX& mtx)
{
   m_d3Device.SetTransform(D3DTS_PROJECTION, &mtx);
}

/////////////////////////////////////////////////////////////////////////////
