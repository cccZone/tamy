#pragma once

/// @file   core-Renderer\EffectShader.h
/// @brief  effect shader abstraction

#include <core\Array.h>
#include "core\Resource.h"
#include "core-Renderer\RenderResource.h"
#include "core-Renderer\ShaderRenderCommand.h"
#include <string>
#include <vector>
#include <map>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * A shader class represents a program written in HLSL.
 */
class EffectShader : public Resource, public RenderResource
{
   DECLARE_RESOURCE( EffectShader )

private:
   std::string m_fileName;
   std::string m_script;

public:
   /**
    * Constructor loading a shader from an .fx file
    *
    * @param resourceName   name of a file with an HLSL shader
    */
   EffectShader( const FilePath& resourceName = FilePath()  );
   virtual ~EffectShader();

   /**
    * Returns the HLSL script of this shader.
    *
    * @return  HLSL shader script
    */
   inline const std::string& getScript() const { return m_script; }

   /**
    * Sets the new shader script.
    *
    * @param script
    */
   void setScript( const std::string& script );

   /**
    * Creates a texture setting shader parameter for the effect shader.
    */
   static ShaderParam< EffectShader >* createTextureSetter( const std::string& paramName, ShaderTexture& val );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Render commands that binds an effect to a render device.
 */
class RCBindEffect : public ShaderRenderCommand< EffectShader >
{
private:
   EffectShader&              m_shader;

   std::string                m_techniqueName;

public:
   RCBindEffect( EffectShader& shader ) : m_shader( shader ) {}

   /**
    * Sets the specified rendering technique.
    *
    * @param techniqueName
    */
   inline void setTechnique( const std::string& techniqueName ) { m_techniqueName = techniqueName; }

   // -------------------------------------------------------------------------
   // ShaderRenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Render commands that unbinds an effect from a render device.
 */
class RCUnbindEffect : public RenderCommand
{
private:
   EffectShader&        m_shader;

public:
   RCUnbindEffect( EffectShader& shader ) : m_shader( shader ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
