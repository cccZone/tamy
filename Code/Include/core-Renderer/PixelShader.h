/// @file   core-Renderer/PixelShader.h
/// @brief  a pixel shader resource
#ifndef _PIXEL_SHADER_H
#define _PIXEL_SHADER_H

#include "core/Resource.h"
#include "core-Renderer/ShaderRenderCommand.h"
#include "core/UniqueObject.h"
#include "core-Renderer/RenderResource.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class RCBindPixelShader;
class InStream;
class OutStream;

///////////////////////////////////////////////////////////////////////////////

/**
 * Texture mapping params
 */
struct TextureStageParams
{
   D3DTEXTUREADDRESS             m_addressU;
   D3DTEXTUREADDRESS             m_addressV;
   D3DTEXTUREADDRESS             m_addressW;
   D3DTEXTUREFILTERTYPE          m_minFilter;
   D3DTEXTUREFILTERTYPE          m_magFilter;
   D3DTEXTUREFILTERTYPE          m_mipFilter;

   TextureStageParams()
      : m_addressU( D3DTADDRESS_WRAP )
      , m_addressV( D3DTADDRESS_WRAP )
      , m_addressW( D3DTADDRESS_WRAP )
      , m_minFilter( D3DTEXF_LINEAR )
      , m_magFilter( D3DTEXF_LINEAR )
      , m_mipFilter( D3DTEXF_LINEAR )
   {}

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend OutStream& operator<<( OutStream& stream, const TextureStageParams& params );
   friend InStream& operator>>( InStream& stream, TextureStageParams& params );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Drawing params
 */
struct PixelShaderParams
{
   D3DCULL                       m_cullingMode;
   bool                          m_useZBuffer;
   bool                          m_writeToZBuffer;

   PixelShaderParams()
      : m_cullingMode( D3DCULL_CCW )
      , m_useZBuffer( true )
      , m_writeToZBuffer( true )
   {}

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend OutStream& operator<<( OutStream& stream, const PixelShaderParams& params );
   friend InStream& operator>>( InStream& stream, PixelShaderParams& params );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a pixel shader program resource.
 */
class PixelShader : public Resource, public UniqueObject< PixelShader >, public RenderResource
{
   DECLARE_RESOURCE()

private:
   // static data
   std::string                               m_script;
   std::string                               m_entryFunctionName;
   PixelShaderParams                         m_params;
   std::vector< TextureStageParams >         m_textureStages;

   // runtime data
   std::vector< std::string >                m_textureStageName;

public:
   /**
    * Constructor.
    *
    * @param resourceName             shader resource name
    */
   PixelShader( const FilePath& resourceName = FilePath() );
   ~PixelShader();

   /**
    * Returns the HLSL script of this shader.
    *
    * @return  HLSL shader script
    */
   inline const std::string& getScript() const { return m_script; }

   /**
    * Sets a new script.
    *
    * @param script
    */
   void setScript( const std::string& script );

   /**
    * Returns the number of texture stages count.
    */
   inline unsigned int getTextureStagesCount() const { return m_textureStages.size(); }

   /**
    * Returns the params used for texture mapping.
    *
    * @param stageIdx
    */
   inline const TextureStageParams& getTextureStage( unsigned int stageIdx ) const { return m_textureStages[stageIdx]; }

   /**
    * Returns the params used for texture mapping (non-const version) so they can be changed.
    *
    * @param stageIdx
    */
   inline TextureStageParams& changeTextureStage( unsigned int stageIdx ) { setDirty(); return m_textureStages[stageIdx]; }

   /**
    * Returns the name of a texture stage.
    *
    * @param stageIdx
    */
   inline const std::string getTextureStageName( unsigned int stageIdx ) const { return m_textureStageName[stageIdx]; }

   /**
    * Returns the drawing params.
    */
   inline const PixelShaderParams& getParams() const { return m_params; }

   /**
    * Returns the drawing params (non-const version) so they can be changed.
    */
   inline PixelShaderParams& changeParams() { setDirty(); return m_params; }

   /**
    * Returns the name of the shader entry function
    */
   inline const std::string& getEntryFunctionName() const { return m_entryFunctionName; }

   /**
    * Creates a texture setting shader parameter for the effect shader.
    */
   static ShaderParam< PixelShader >* createTextureSetter( const std::string& paramName, ShaderTexture& val );

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded( ResourcesManager& mgr );

private:
   void parseTextureStages();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Binds a pixel shader to the render device.
 */
class RCBindPixelShader : public ShaderRenderCommand< PixelShader >
{
private:
   PixelShader&               m_shader;
   
public:
   /**
    * Constructor.
    */
   RCBindPixelShader( PixelShader& shader ) : m_shader( shader ) {}

   // -------------------------------------------------------------------------
   // Shader program setters
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Binds a pixel shader to the render device.
 */
class RCUnbindPixelShader : public RenderCommand
{
private:
   PixelShader&         m_shader;

public:
   /**
    * Constructor.
    */
   RCUnbindPixelShader( PixelShader& shader ) : m_shader( shader ) {}

   // -------------------------------------------------------------------------
   // Shader program setters
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

#endif // _PIXEL_SHADER_H
