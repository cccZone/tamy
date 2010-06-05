#pragma once

/// @file   core-Renderer\ShaderTexture.h
/// @brief  common interface that allows to use various types of textures
///         as a shader param


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
