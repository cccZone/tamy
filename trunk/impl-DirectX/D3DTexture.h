#pragma once

#include "Texture.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * A DirectX representation of a texture
 */
class D3DTexture : public Texture
{
private:
   IDirect3DDevice9& m_d3Device;
   IDirect3DTexture9* m_texture;

public:
   D3DTexture(const std::string& name, 
              IDirect3DDevice9& d3Device, 
              IDirect3DTexture9& texture);
   ~D3DTexture();

   void setForRendering();
};

///////////////////////////////////////////////////////////////////////////////
