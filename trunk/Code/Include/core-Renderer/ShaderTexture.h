#pragma once

/// @file   core-Renderer\ShaderTexture.h
/// @brief  common interface that allows to use various types of textures
///         as a shader param


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

class ShaderTexture
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
};

///////////////////////////////////////////////////////////////////////////////
