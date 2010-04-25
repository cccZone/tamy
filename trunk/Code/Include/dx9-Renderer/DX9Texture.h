#pragma once

/// @file   dx9-Renderer\DX9Texture.h
/// @brief  directX9 implementation of a texture

#include "core-Renderer\Texture.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////


class DX9Texture : public TextureImpl
{
private:
   Texture& m_texture;
   IDirect3DTexture9* m_dxTexture;

public:
   DX9Texture(Texture& texture);
   ~DX9Texture();

   // -------------------------------------------------------------------------
   // RendererObjectImpl implementation
   // -------------------------------------------------------------------------
   void initialize(Renderer& renderer);

   // -------------------------------------------------------------------------
   // TextureImpl implementation
   // -------------------------------------------------------------------------
   void* getPlatformSpecific() const;
};

///////////////////////////////////////////////////////////////////////////////
