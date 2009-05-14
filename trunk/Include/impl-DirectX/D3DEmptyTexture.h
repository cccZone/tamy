#pragma once

#include "core-Renderer\Texture.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * An empty texture
 */
class D3DEmptyTexture : public Texture
{
private:
   IDirect3DDevice9& m_d3Device;

public:
   D3DEmptyTexture(IDirect3DDevice9& d3Device);
   ~D3DEmptyTexture();

   void setForRendering(unsigned char stageIndex);
};

///////////////////////////////////////////////////////////////////////////////
