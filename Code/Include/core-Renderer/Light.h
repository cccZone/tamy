/// @file   core-Renderer/Light.h
/// @brief  base class and render commands for lights
#pragma once

#include "core-MVC/SpatialEntity.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class ShaderTexture;
class RenderTarget;
class RenderingView;

///////////////////////////////////////////////////////////////////////////////

/**
 * Base class for the light.
 */
class Light : public SpatialEntity
{
   DECLARE_CLASS();

protected:
   bool           m_castsShadows;

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
    * @param depthNormalsTex        a texture containing scene depth and normals
    * @param sceneColorTex          a texture with rendered scene colors ( coming from the materials of scene objects )
    */
   virtual void renderLighting( Renderer& renderer, ShaderTexture* depthNormalsTex, ShaderTexture* sceneColorTex ) {}

   /**
    * Renders a shadow mp of the shadows cast by this light.
    *
    * @param renderer
    * @param shadowDepthBuffer      a render target to which the scene depth seen from the light's perspective will be rendered
    * @param screenSpaceShadowMap   output shadow map seen from the active camera's perspective
    * @param geometryToRender       geometry to be rendered
    */
   virtual void renderShadowMap( Renderer& renderer, RenderTarget* shadowDepthBuffer, RenderTarget* screenSpaceShadowMap, const RenderingView* renderedSceneView ) {}

   /**
    * Tells if the light is set to cast shadows.
    */
   inline bool castsShadows() const { return m_castsShadows; }
};

///////////////////////////////////////////////////////////////////////////////
