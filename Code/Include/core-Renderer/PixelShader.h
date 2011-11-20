/// @file   core-Renderer/PixelShader.h
/// @brief  a pixel shader resource

#pragma once

#include "core/Resource.h"
#include "core-Renderer/ShaderRenderCommand.h"
#include "core/UniqueObject.h"
#include "core-Renderer\RenderResource.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class RCBindPixelShader;
class PixelShaderConstant;

///////////////////////////////////////////////////////////////////////////////

/**
 * Texture mapping params
 */
struct PixelShaderParams
{
   D3DCULL                       m_cullingMode;
   bool                          m_useZBuffer;
   bool                          m_writeToZBuffer;
   D3DTEXTUREADDRESS             m_addressU;
   D3DTEXTUREADDRESS             m_addressV;
   D3DTEXTUREADDRESS             m_addressW;
   D3DTEXTUREFILTERTYPE          m_minFilter;
   D3DTEXTUREFILTERTYPE          m_magFilter;
   D3DTEXTUREFILTERTYPE          m_mipFilter;

   PixelShaderParams()
      : m_cullingMode( D3DCULL_CCW )
      , m_useZBuffer( true )
      , m_writeToZBuffer( true )
      , m_addressU( D3DTADDRESS_WRAP )
      , m_addressV( D3DTADDRESS_WRAP )
      , m_addressW( D3DTADDRESS_WRAP )
      , m_minFilter( D3DTEXF_LINEAR )
      , m_magFilter( D3DTEXF_LINEAR )
      , m_mipFilter( D3DTEXF_LINEAR )
   {}

   void serialize( Serializer& serializer )
   {
      serializer << m_cullingMode;
      serializer << m_useZBuffer;
      serializer << m_writeToZBuffer;
      serializer << m_addressU;
      serializer << m_addressV;
      serializer << m_addressW;
      serializer << m_minFilter;
      serializer << m_magFilter;
      serializer << m_mipFilter;
   }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a pixel shader program resource.
 */
class PixelShader : public Resource, public UniqueObject< PixelShader >, public RenderResource
{
   DECLARE_RESOURCE( PixelShader )

private:
   // static data
   std::string                               m_script;
   std::string                               m_entryFunctionName;
   PixelShaderParams                         m_params;

   // runtime data
   std::vector< PixelShaderConstant* >       m_constants;

public:
   /**
    * Constructor.
    *
    * @param scriptPath             shader resource name
    */
   PixelShader( const std::string& fileName = "" );
   ~PixelShader();

   /**
    * Loads a script from a file.
    *
    * @param fs                  file system that contains the file
    * @param fileName            name of the file with the shader's code
    * @param entryFunctionName   name of a function in the file that's to be used as the shader's entry function
    */
   void loadFromFile( const Filesystem& fs, const std::string& fileName, const std::string& entryFunctionName = "main" );

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
    * Returns the params used for texture mapping.
    */
   inline const PixelShaderParams& getParams() const { return m_params; }

   /**
    * Returns the params used for texture mapping (non-const version) so they can be changed.
    */
   inline PixelShaderParams& changeParams() { setDirty(); return m_params; }

   /**
    * Returns the name of the shader entry function
    */
   inline const std::string& getEntryFunctionName() const { return m_entryFunctionName; }

   /**
    * Returns shader constants.
    */
   inline const std::vector< PixelShaderConstant* >& getConstants() const { return m_constants; }

   /**
    * Creates a texture setting shader parameter for the effect shader.
    */
   static ShaderParam< PixelShader >* createTextureSetter( const std::string& paramName, ShaderTexture& val );

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded( ResourcesManager& mgr );

private:
   void parseShaderConstants();
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