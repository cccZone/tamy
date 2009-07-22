#pragma once

#include "core-Renderer\Texture.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

template<>
class TTextureImplDestructor<IDirect3DTexture9>
{
public:
   void destroy(IDirect3DTexture9* tex)
   {
      if (tex != NULL)
      {
         tex->Release();
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
