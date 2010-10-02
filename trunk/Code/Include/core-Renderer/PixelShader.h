/// @file   core-Renderer/PixelShader.h
/// @brief  a pixel shader resource

#pragma once

#include "core-Renderer/RendererObject.h"
#include "core-Renderer/RendererObjectImpl.h"
#include "core/Resource.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class PixelShaderImpl;
class ShaderTexture;
class Filesystem;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a pixel shader program resource.
 */
class PixelShader : public Resource, public TRendererObject< PixelShaderImpl >
{
   DECLARE_RESOURCE( PixelShader )

private:
   std::string                   m_script;

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
