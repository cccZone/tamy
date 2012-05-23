/// @file   core-Renderer/Shader.h
/// @brief  a vertex shader resource

#pragma once

#include "core-Renderer/VertexDescriptions.h"
#include "core-Renderer\ShaderRenderCommand.h"
#include "core/Resource.h"
#include "core/UniqueObject.h"
#include "core-Renderer\RenderResource.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a vertex shader program resource.
 */
class VertexShader : public Resource, public UniqueObject< VertexShader >, public RenderResource
{
   DECLARE_RESOURCE()

private:
   std::string                   m_script;
   std::string                   m_entryFunctionName;
   VertexDescId                  m_vertexDescId;

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   VertexShader( const FilePath& resourceName = FilePath() );

   /**
    * Sets a new vertex description. Supported only for vertex shaders.
    */
   void setVertexDescription( VertexDescId vertexDescId );

   /**
    * Returns the description of the vertex the shader supports( providing it's vertex shader )
    */
   inline const VertexDescriptor* getVerexDescription() const { return g_vertexDescriptions[ m_vertexDescId ]; }

   /**
    * Returns the HLSL script of this shader.
    *
    * @return  HLSL shader script
    */
   inline const std::string& getScript() const { return m_script; }

   /**
    * Sets a new HLSL script.
    *
    * @param script
    */
   inline void setScript( const std::string& script ) { m_script = script; setDirty(); }

   /**
    * Returns the name of the shader entry function
    */
   inline const std::string& getEntryFunctionName() const { return m_entryFunctionName; }

   /**
    * Creates a texture setting shader parameter for the effect shader.
    */
   static ShaderParam< VertexShader >* createTextureSetter( const std::string& paramName, ShaderTexture& val );
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
