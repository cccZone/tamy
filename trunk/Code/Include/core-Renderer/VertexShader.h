/// @file   core-Renderer/Shader.h
/// @brief  a vertex shader resource

#pragma once

#include "core-Renderer/VertexDescriptions.h"
#include "core-Renderer\ShaderRenderCommand.h"
#include "core/Resource.h"
#include "core/UniqueObject.h"
#include "core-Renderer\RenderResource.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a vertex shader program resource.
 */
class VertexShader : public Resource, public UniqueObject< VertexShader >, public RenderResource
{
   DECLARE_RESOURCE( VertexShader )

private:
   std::string                   m_script;
   VertexDescId                  m_vertexDescId;

   // runtime data
   std::string                   m_scriptPath;

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
    * Creates a texture setting shader parameter for the effect shader.
    */
   static ShaderParam< VertexShader >* createTextureSetter( const std::string& paramName, ShaderTexture& val );

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded(ResourcesManager& mgr);
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Binds a vertex shader to a rendering device.
 */
class RCBindVertexShader : public ShaderRenderCommand< VertexShader >
{
private:
   VertexShader&              m_shader;

public:
   RCBindVertexShader( VertexShader& shader ) : m_shader( shader ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Unbinds a vertex shader from a rendering device.
 */
class RCUnbindVertexShader : public RenderCommand
{
private:
   VertexShader&           m_shader;

public:
   RCUnbindVertexShader( VertexShader& shader ) : m_shader( shader ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
