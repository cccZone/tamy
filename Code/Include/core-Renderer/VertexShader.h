/// @file   core-Renderer/Shader.h
/// @brief  a vertex shader resource

#pragma once

#include "core-Renderer\VertexDescriptions.h"
#include "core-Renderer\ShaderRenderCommand.h"
#include "core-Renderer\RenderResource.h"
#include "core-Renderer\ShaderConstantDesc.h"
#include "core\Resource.h"
#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a vertex shader program resource.
 */
class VertexShader : public Resource, public UniqueObject< VertexShader >, public RenderResource
{
   DECLARE_ALLOCATOR( VertexShader, AM_DEFAULT );
   DECLARE_RESOURCE()

private:
   std::string                               m_script;
   std::string                               m_entryFunctionName;   // ';'-separated list of entry function names
   std::string                               m_techniqueNames;      // ';'-separated list of technique names
   VertexDescId                              m_vertexDescId;
   std::vector< ShaderConstantDesc >         m_constantsDescriptions;

   // runtime data
   std::vector< std::string >                m_arrEntryFunctionNames;
   std::vector< uint >                       m_arrTechniqueIds;

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

   // -------------------------------------------------------------------------
   // Rendering techniques
   // -------------------------------------------------------------------------
   /**
    * A helper method that generates unique ids for rendering techniques.
    */
   static uint generateTechniqueId( const std::string& techqniueName );

   /**
    * Looks for a technique with the specified id. If the shader doesn't implement such, a negative index will be returned.
    */
   int findTechnique( uint techniqueId ) const;

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
    *
    * @param allocator
    * @param paramName
    * @param val
    */
   static ShaderParam< VertexShader >* createTextureSetter( MemoryPoolAllocator* allocator, const IDString& paramName, ShaderTexture& val );

   /**
    * Returns the descriptions of the compiled shader constants.
    * These descriptions can be passed to a constants compiler to compile them into pipeline node constants.
    */
   const std::vector< ShaderConstantDesc >& getConstantsDescriptions() const { return m_constantsDescriptions; }

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

private:
   void parseTechniques();
   void parseConstants();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Binds a vertex shader to a rendering device.
 */
class RCBindVertexShader : public ShaderRenderCommand< VertexShader >
{
   DECLARE_ALLOCATOR( RCBindVertexShader, AM_DEFAULT );

public:
   VertexShader&              m_shader;
   int                        m_techniqueIdx;

public:
   RCBindVertexShader( VertexShader& shader, Renderer& renderer );

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
   DECLARE_ALLOCATOR( RCUnbindVertexShader, AM_DEFAULT );

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
