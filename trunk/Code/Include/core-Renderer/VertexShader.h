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
   std::string                   m_entryFunctionName;   // ';'-separated list of entry function names
   std::string                   m_techniqueNames;      // ';'-separated list of technique names
   VertexDescId                  m_vertexDescId;

   // runtime data
   std::vector< std::string >    m_arrEntryFunctionNames;
   std::vector< std::string >    m_arrTechniqueNames;

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
    * Returns the id of the vertex description this shader uses.
    */
   inline VertexDescId getVertexDescriptionId() const { return m_vertexDescId; }

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
   void setScript( const std::string& script );

   /**
    * Returns the name of the specified technique
    *
    * @param techniqueIdx
    */
   inline const std::string& getTechiqueName( uint techniqueIdx ) const { return m_arrTechniqueNames[ techniqueIdx ]; }

   /**
    * Returns the name of the shader entry function for the specified technique.
    *
    * @param techniqueIdx
    */
   inline const std::string& getEntryFunctionName( uint techniqueIdx ) const { return m_arrEntryFunctionNames[ techniqueIdx ]; }

   /**
    * Returns the number of techniques the vertex shader can handle
    */
   inline uint getTechniquesCount() const { return m_arrEntryFunctionNames.size(); }

   /**
    * Creates a texture setting shader parameter for the effect shader.
    */
   static ShaderParam< VertexShader >* createTextureSetter( const std::string& paramName, ShaderTexture& val );

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

private:
   void parseTechniques();
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
