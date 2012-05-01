/// @file   core-Renderer\Texture.h
/// @brief  image rendered on a surface
#pragma once

#include "core\Resource.h"
#include "core-Renderer\ShaderTexture.h"
#include "core-Renderer\RenderCommand.h"
#include "core-Renderer\RenderResource.h"
#include "core/UniqueObject.h"
#include <string>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class SingletonsManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * An image that can be drawn on a rendered surface.
 */
class Texture : public Resource, public RenderResource, public UniqueObject< Texture >
{
   DECLARE_RESOURCE()

private:
   FilePath          m_texFileName;
   TextureUsage      m_usage;
   
   unsigned int      m_width;
   unsigned int      m_height;

public:
   /**
    * Constructor.
    */
   Texture( const FilePath& resourceName = FilePath() );
   ~Texture();

   /**
    * Sets the texture path.
    *
    * @param texPath
    */
   inline void setTextureName( const FilePath& texPath ) { setDirty(); m_texFileName = texPath; }

   /**
    * Returns the name of the image file.
    */
   inline const FilePath& getTextureName() const { return m_texFileName; }

   /**
    * Sets new texture usage.
    *
    * @param usage
    */
   inline void setUsage( TextureUsage usage ) { setDirty(); m_usage = usage; }

   /**
    * Returns texture usage.
    */
   inline TextureUsage getUsage() const { return m_usage; }

   /**
    * A texture is just an image. This method returns a pointer
    * to the memory buffer occupied by the image.
    *
    * @param outSize    output parameter that will be initialized
    *                   with the buffer size (in bytes)
    * @param outBuffer  output parameter that will be initialized
    *                   with a pointer to the image memory buffer
    */
   void getBuffer(  byte*& imgBuffer, unsigned int& bufSize ) const;

   /**
    * Returns the width of the texture.
    */
   inline unsigned int getWidth() const { return m_width; }

   /**
    * Returns the height of the texture.
    */
   inline unsigned int getHeight() const { return m_height; }
};

///////////////////////////////////////////////////////////////////////////////
