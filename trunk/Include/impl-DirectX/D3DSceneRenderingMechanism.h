#pragma once

#include "core-Renderer\SceneRenderingMechanism.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DSceneRenderingMechanism : public SceneRenderingMechanism
{
private:
   IDirect3DDevice9& m_d3Device;

public:
   D3DSceneRenderingMechanism(RenderingTargetsPolicy* policy,
                              unsigned int maxLightsCount,
                              MaterialImpl* impl,
                              IDirect3DDevice9& d3Device);

protected:
   void setViewMatrix(const D3DXMATRIX& mtx);
   void setProjectionMatrix(const D3DXMATRIX& mtx);
};

///////////////////////////////////////////////////////////////////////////////
