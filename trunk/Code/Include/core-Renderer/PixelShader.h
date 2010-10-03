/// @file   core-Renderer/PixelShader.h
/// @brief  a pixel shader resource

#pragma once

#include "core-Renderer/RendererObject.h"
#include "core-Renderer/RendererObjectImpl.h"
#include "core/Resource.h"
#include <d3dx9.h>

// TODO: !!!!!!!!!!!! Entity do ktorego mozna podlaczac ten konfigurowalny shader

///////////////////////////////////////////////////////////////////////////////

class PixelShaderImpl;
class ShaderTexture;
class Filesystem;

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
class PixelShader : public Resource, public TRendererObject< PixelShaderImpl >
{
   DECLARE_RESOURCE( PixelShader )

private:
   std::string                   m_script;
   PixelShaderParams             m_params;

public:
   /**
    * Constructor.
    *
    * @param scriptPath    path to the .psh file containing the shader's HLSL code
    */
   PixelShader( const std::string& fileName = "" );

   /**
    * Loads a script from a file.
    */
   void loadFromFile( const Filesystem& fs, const std::string& fileName );

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
   inline void setScript( const std::string& script ) { m_script = script; }

   /**
    * Returns the params used for texture mapping.
    */
   inline const PixelShaderParams& getParams() const { return m_params; }

   /**
    * Returns the params used for texture mapping (non-const version)
    */
   inline PixelShaderParams& getParams() { return m_params; }

   /**
    * Starts the rendering process.
    */
   void beginRendering();

   /**
    * Ends the rendering process.
    */
   void endRendering();

   // -------------------------------------------------------------------------
   // Shader program setters
   // -------------------------------------------------------------------------
   void setBool( const char* paramName, bool val );

   void setMtx( const char* paramName, const D3DXMATRIX& matrix );

   void setMtxArray( const char* paramName, const D3DXMATRIX* matrices, unsigned int count );

   void setVec4( const char* paramName, const D3DXVECTOR4& vec );

   void setTexture( const char* paramName, ShaderTexture& val );

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onComponentAdded( Component< ResourcesManager >& component );
   void onComponentRemoved( Component< ResourcesManager >& component );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A pixel shader implementation interface.
 */
class PixelShaderImpl : public RendererObjectImpl
{
public:
   virtual ~PixelShaderImpl() {}

   virtual void beginRendering() {}

   virtual void endRendering() {}

   virtual void setBool( const char* paramName, bool val ) {}

   virtual void setMtx( const char* paramName, const D3DXMATRIX& matrix ) {}

   virtual void setMtxArray( const char* paramName, const D3DXMATRIX* matrices, unsigned int count ) {}

   virtual void setVec4( const char* paramName, const D3DXVECTOR4& vec ) {}

   virtual void setTexture( const char* paramName, ShaderTexture& val ) {}
};

///////////////////////////////////////////////////////////////////////////////
