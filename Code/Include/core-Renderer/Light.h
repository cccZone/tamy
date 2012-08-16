/// @file   core-Renderer/Light.h
/// @brief  base class and render commands for lights
#pragma once

#include "core-MVC/SpatialEntity.h"
#include "core/Array.h"

///////////////////////////////////////////////////////////////////////////////

class Renderer;
class ShaderTexture;
class RenderTarget;
class DepthBuffer;
class RenderingView;
class Geometry;

///////////////////////////////////////////////////////////////////////////////

/**
 * A helper structure that aggregates all data needed to render shadows.
 */
struct ShadowRendererData
{
   const RenderingView*             m_renderingView;
   const Array< Geometry* >*        m_geometryToRender;
   RenderTarget*                    m_shadowDepthTexture;
   DepthBuffer*                     m_shadowDepthSurface;
   RenderTarget*                    m_screenSpaceShadowMap;
};

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
    * @param data
    */
   virtual void renderShadowMap( Renderer& renderer, const ShadowRendererData& data ) {}

   /**
    * Tells if the light is set to cast shadows.
    */
   inline bool castsShadows() const { return m_castsShadows; }
};

///////////////////////////////////////////////////////////////////////////////
