#pragma once

/// @file   core-Renderer\ShaderTexture.h
/// @brief  common interface that allows to use various types of textures
///         as a shader param

#include "core\Subject.h"
#include "core-Renderer\ShaderParam.h"
#include "core/Enum.h"


///////////////////////////////////////////////////////////////////////////////

class EffectShader;
class PixelShader;
class VertexShader;

///////////////////////////////////////////////////////////////////////////////

/**
 * Operations performed on a texture we can listen to.
 */
enum ShaderTextureOps
{
   STO_RESIZE
};


///////////////////////////////////////////////////////////////////////////////

/**
 * Describes the usage of the texture.
 */
enum TextureUsage
{
   TU_COLOR,
   TU_DEPTH,
   TU_LUMINANCE,
   TU_HDR,
   TU_DATA_TEXTURE,

   TU_MAX
};

///////////////////////////////////////////////////////////////////////////////

class ShaderTexture : public Subject< ShaderTexture, ShaderTextureOps >
{
public:
   virtual ~ShaderTexture() {}

   /**
    * Returns the width of the texture.
    */
   virtual unsigned int getWidth() const = 0;

   /**
    * Returns the height of the texture.
    */
   virtual unsigned int getHeight() const = 0;

   // -------------------------------------------------------------------------
   // Shader params setters creating commands ( Visitor pattern )
   // -------------------------------------------------------------------------
   /**
    * Creates a texture setter for an effect shader.
    */
   virtual ShaderParam< EffectShader >* createEffectShaderTextureSetter( const std::string& paramName ) = 0;

   /**
    * Creates a texture setter for a pixel shader.
    */
   virtual ShaderParam< PixelShader >* createPixelShaderTextureSetter( const std::string& paramName ) = 0;

   /**
    * Creates a texture setter for a vertex shader.
    */
   virtual ShaderParam< VertexShader >* createVertexShaderTextureSetter( const std::string& paramName ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
