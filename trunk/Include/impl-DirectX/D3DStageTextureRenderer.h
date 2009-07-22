#pragma once

#include "core-Renderer\StageTextureRenderer.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DStageTextureRenderer : public StageTextureRenderer
{
private:
   IDirect3DDevice9& m_d3Device;

public:
   D3DStageTextureRenderer(IDirect3DDevice9& d3Device);

   void setForRendering(unsigned int stageIdx, TextureImpl& textureImpl);
};

///////////////////////////////////////////////////////////////////////////////
