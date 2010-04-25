#pragma once

/// @file   core-Renderer\EffectResource.h
/// @brief  resource for loading effects based on HLSL shaders

#include <core\Array.h>
#include "core\Resource.h"
#include "core-Renderer\RendererObject.h"


///////////////////////////////////////////////////////////////////////////////

class Shader;
class Effect;
class Filesystem;
class SingletonsManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * A resource for creating an effect based on an HLSL shader.
 * Shader class type is matched based on the shader file name. 
 *
 * So if the shader file is named "SomeShader.fx", you're gonna
 * need to have a SomeShader class present and registered with
 * the entities reflection system.
 *
 * The file name can contain a relative path - the resource will strip that
 * off while extracting the name of the effect class.
 */
class EffectResource : public Resource
{
private:
   Shader* m_shader;
   std::string m_fileName;
   std::string m_entityClassName;

public:
   /**
    * Resource-compliant constructor.
    */
   EffectResource(Filesystem& fs, 
      const std::string& fileName);

   /**
    * Creates a new instance of the effect described by the resource.
    */
   Effect* load();

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onLoaded(ResourcesManager& mgr);
};

///////////////////////////////////////////////////////////////////////////////
