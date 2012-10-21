/// @file   core-Renderer\RenderableTexture.h
/// @brief  An adapter that allows to plug a texture image into a shader
#pragma once

#include "core-Renderer\ShaderTexture.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * An adapter that allows to plug a texture image into a shader.
 */
class RenderableTexture : public ShaderTexture
{
   DECLARE_ALLOCATOR( RenderableTexture, AM_DEFAULT );

private:
   Texture*    m_texture;

public:
   /**
    * Constructor.
    */
   RenderableTexture();

   /**
    * Sets a new texture the adapter should proxy the calls to.
    */
   void setTexture( Texture* texture );

   // -------------------------------------------------------------------------
   // ShaderTexture implementation
   // -------------------------------------------------------------------------
   unsigned int getWidth() const;
   unsigned int getHeight() const;
   ShaderParam< EffectShader >* createEffectShaderTextureSetter( MemoryPoolAllocator* allocator, const IDString& paramName );
   ShaderParam< PixelShader >* createPixelShaderTextureSetter( MemoryPoolAllocator* allocator, const IDString& paramName );
   ShaderParam< VertexShader >* createVertexShaderTextureSetter( MemoryPoolAllocator* allocator, const IDString& paramName );
};

///////////////////////////////////////////////////////////////////////////////
