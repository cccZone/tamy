/// @file   core-Renderer/AmbientLight.h
/// @brief  an entity that provides a uniform, ambient lighting for the scene
#pragma once

#include "core-Renderer\Light.h"
#include "core\Color.h"
#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;

///////////////////////////////////////////////////////////////////////////////

/**
 * An entity that provides a uniform, ambient lighting for the scene.
 */
class AmbientLight : public SpatialEntity, public UniqueObject< AmbientLight >
{
   DECLARE_CLASS();

public:
   Color             m_lightColor;

   PixelShader*      m_shader;

public:
   /**
    * Constructor.
    */
   AmbientLight( const std::string& name = "" );
   ~AmbientLight();

   /**
    * Renders the ambient light.
    *
    * @param renderer
    * @param sceneColorTex       a texture with raw scene albedo
    * @param renderTarget        render target to which the lit scene should be drawn
    */
   void render( Renderer& renderer, ShaderTexture* sceneColorTex, RenderTarget* renderTarget );

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

private:
   void initialize();
};

///////////////////////////////////////////////////////////////////////////////
