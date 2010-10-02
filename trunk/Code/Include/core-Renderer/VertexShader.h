/// @file   core-Renderer/Shader.h
/// @brief  a vertex shader resource

#pragma once

#include "core-Renderer/RendererObject.h"
#include "core-Renderer/RendererObjectImpl.h"
#include "core-Renderer/VertexDescriptions.h"
#include "core/Resource.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class VertexShaderImpl;
class ShaderTexture;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a vertex shader program resource.
 */
class VertexShader : public Resource, public TRendererObject< VertexShaderImpl >
{
   DECLARE_RESOURCE( VertexShader )

private:
   std::string                   m_script;
   VertexDescId                  m_vertexDescId;

public:
   /**
    * Constructor.
    */
   VertexShader();

   /**
    * Constructor.
    *
    * @param scriptPath    path to the .vsh file containing the shader's HLSL code
    */
   VertexShader( const std::string& scriptPath );

   /**
    * Sets a new vertex description. Supported only for vertex shaders.
    */
   void setVertexDescription( VertexDescId vertexDescId );

   /**
    * Returns the description of the vertex the shader supports( providing it's vertex shader )
    */
   inline const D3DVERTEXELEMENT9* getVerexDescription() const { return g_vertexDescriptions[ m_vertexDescId ]; }

   /**
    * Returns the HLSL script of this shader.
    *
    * @return  HLSL shader script
    */
   inline const std::string& getScript() const { return m_script; }

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
   void onResourceLoaded(ResourcesManager& mgr);
   void onComponentAdded( Component< ResourcesManager >& component );
   void onComponentRemoved( Component< ResourcesManager >& component );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A vertex shader implementation interface.
 */
class VertexShaderImpl : public RendererObjectImpl
{
public:
   virtual ~VertexShaderImpl() {}

   virtual void beginRendering() {}

   virtual void endRendering() {}

   virtual void setBool( const char* paramName, bool val ) {}

   virtual void setMtx( const char* paramName, const D3DXMATRIX& matrix ) {}

   virtual void setMtxArray( const char* paramName, const D3DXMATRIX* matrices, unsigned int count ) {}

   virtual void setVec4( const char* paramName, const D3DXVECTOR4& vec ) {}

   virtual void setTexture( const char* paramName, ShaderTexture& val ) {}
};

///////////////////////////////////////////////////////////////////////////////
