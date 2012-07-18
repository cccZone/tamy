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
   ShaderParam< EffectShader >* createEffectShaderTextureSetter( const std::string& paramName );
   ShaderParam< PixelShader >* createPixelShaderTextureSetter( const std::string& paramName );
   ShaderParam< VertexShader >* createVertexShaderTextureSetter( const std::string& paramName );
};

///////////////////////////////////////////////////////////////////////////////
