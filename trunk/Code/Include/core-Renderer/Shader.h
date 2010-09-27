/// @file   core-Renderer/Shader.h
/// @brief  a vertex shader resource

#pragma once

#include "core-Renderer/RendererObject.h"
#include "core-Renderer/RendererObjectImpl.h"
#include "core-Renderer/VertexDescriptions.h"
#include "core/Resource.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class ShaderImpl;
class ShaderTexture;

///////////////////////////////////////////////////////////////////////////////

enum ShaderType
{
   SHT_VERTEX_SHADER,
   SHT_PIXEL_SHADER,
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a vertex shader program resource.
 */
class Shader : public Resource, public TRendererObject< ShaderImpl >
{
   DECLARE_RESOURCE( Shader )

private:
   ShaderType                    m_type;
   std::string                   m_script;
   VertexDescId                  m_vertexDescId;

public:
   /**
    * Constructor.
    */
   Shader();

   /**
    * Constructor.
    *
    * @param scriptPath    path to the .vsh file containing the shader's HLSL code
    */
   Shader( const std::string& scriptPath, ShaderType type );

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
    * Returns the type of the shader.
    */
   inline ShaderType getType() const { return m_type; }

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
class ShaderImpl : public RendererObjectImpl
{
public:
   virtual ~ShaderImpl() {}

   virtual void beginRendering() {}

   virtual void endRendering() {}

   virtual void setBool( const char* paramName, bool val ) {}

   virtual void setMtx( const char* paramName, const D3DXMATRIX& matrix ) {}

   virtual void setMtxArray( const char* paramName, const D3DXMATRIX* matrices, unsigned int count ) {}

   virtual void setVec4( const char* paramName, const D3DXVECTOR4& vec ) {}

   virtual void setTexture( const char* paramName, ShaderTexture& val ) {}
};

///////////////////////////////////////////////////////////////////////////////
