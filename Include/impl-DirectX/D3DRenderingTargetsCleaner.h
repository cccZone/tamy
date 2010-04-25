#pragma once

#include <d3d9.h>
#include "core-Renderer\RenderingTargetsCleaner.h"


///////////////////////////////////////////////////////////////////////////////

class D3DRenderingTargetsCleaner : public RenderingTargetsCleaner
{
private:
   IDirect3DDevice9& m_d3Device;

public:
   D3DRenderingTargetsCleaner(IDirect3DDevice9& d3Device);

   void cleanRenderingTargets(unsigned int targetsCount);
};

///////////////////////////////////////////////////////////////////////////////
