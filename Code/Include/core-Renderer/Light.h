/// @file   core-Renderer/Light.h
/// @brief  base class and render commands for lights
#pragma once

#include "core-MVC/SpatialEntity.h"
#include "core-Renderer/RenderCommand.h"
#include "core/Array.h"


///////////////////////////////////////////////////////////////////////////////

class ShaderTexture;

///////////////////////////////////////////////////////////////////////////////

/**
 * Base class for the light.
 */
class Light : public SpatialEntity
{
   DECLARE_CLASS();

public:
   /**
    * Constructor.
    *
    * @param name
    */
   Light( const std::string& name = "" );
   virtual ~Light() {}

   /**
    * Renders the light volume.
    *
    * @param renderer
    * @param depthNormalsBufferTex        a texture containing scene depth and normals
    */
   virtual void render( Renderer& renderer, ShaderTexture* depthNormalsBufferTex ) {}
};

///////////////////////////////////////////////////////////////////////////////
