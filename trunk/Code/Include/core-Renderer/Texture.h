#pragma once

/// @file   core-Renderer\Texture.h
/// @brief  image rendered on a surface

#include "core\Resource.h"
#include "core-Renderer\RenderState.h"
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
class Texture : public Resource, public TRenderState< Texture >, public ShaderTexture, public UniqueObject< Texture >, public RenderResource
{
   DECLARE_RESOURCE( Texture )

private:
   std::string       m_texFileName;
   
   unsigned int      m_width;
   unsigned int      m_height;

public:
   /**
    * Constructor.
    */
   Texture( const std::string& fileName = "" );
   ~Texture();

   /**
    * Returns the name of the image file.
    */
   inline const std::string& getTextureName() const { return m_texFileName; }

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

   // -------------------------------------------------------------------------
   // RenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer );
   void onPostRender( Renderer& renderer );
   bool onEquals( const Texture& rhs ) const;
   bool onLess( const Texture& rhs ) const;

   // -------------------------------------------------------------------------
   // ShaderTexture implementation
   // -------------------------------------------------------------------------
   inline unsigned int getWidth() const { return m_width; }
   inline unsigned int getHeight() const { return m_height; }
   inline ShaderParam< EffectShader >* createEffectShaderTextureSetter( const std::string& paramName ) { return new ShaderParamTexture< EffectShader >( paramName, *this ); }
   inline ShaderParam< PixelShader >* createPixelShaderTextureSetter( const std::string& paramName ) { return new ShaderParamTexture< PixelShader >( paramName, *this ); }
   inline ShaderParam< VertexShader >* createVertexShaderTextureSetter( const std::string& paramName ){ return new ShaderParamTexture< VertexShader >( paramName, *this ); }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command that ensures a texture existence.
 */
class RCCreateTexture : public RenderCommand
{
private:
   Texture&          m_texture;
   unsigned int&     m_width;
   unsigned int&     m_height;

public:
   RCCreateTexture( Texture& texture, unsigned int& width, unsigned int& height ) 
      : m_texture( texture )
      , m_width( width ) 
      , m_height( height )
   {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
