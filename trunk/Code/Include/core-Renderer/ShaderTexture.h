#pragma once

/// @file   core-Renderer\ShaderTexture.h
/// @brief  common interface that allows to use various types of textures
///         as a shader param

#include "core/Subject.h"


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

   TU_MAX
};

///////////////////////////////////////////////////////////////////////////////

class ShaderTexture : public Subject< ShaderTexture, ShaderTextureOps >
{
public:
   virtual ~ShaderTexture() {}

   /**
    * Returns a pointer to the platform specific implementation
    * of the texture.
    *
    * Only the implementation will know what to do with it.
    *
    * @return     pointer to impl-specific texture structure
    */
   virtual void* getPlatformSpecific() const = 0;

   /**
    * Returns the width of the texture.
    */
   virtual unsigned int getWidth() const = 0;

   /**
    * Returns the height of the texture.
    */
   virtual unsigned int getHeight() const = 0;
};

///////////////////////////////////////////////////////////////////////////////
