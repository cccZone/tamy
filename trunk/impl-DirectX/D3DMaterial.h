#pragma once

#include "Material.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a DirectX imlpementation of a material related functionality
 */
class D3DMaterial : public Material
{
private:
   IDirect3DDevice9& m_d3Device;

public:
   D3DMaterial(Texture& emptyTexture, unsigned int index, 
               IDirect3DDevice9& d3Device);

protected:
   void enableTransparency(bool enable);
};

///////////////////////////////////////////////////////////////////////////////
