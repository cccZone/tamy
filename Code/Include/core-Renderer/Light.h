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
struct LightingRenderData
{
   // data required by the shadow calculation shaders
   const RenderingView*             m_renderingView;
   const Array< Geometry* >*        m_geometryToRender;
   RenderTarget*                    m_shadowDepthTexture;
   DepthBuffer*                     m_shadowDepthSurface;
   RenderTarget*                    m_screenSpaceShadowMap;

   // data required by the lighting calculation shaders
   ShaderTexture*                   m_depthNormalsTex;
   ShaderTexture*                   m_sceneColorTex;
   RenderTarget*                    m_finalLightColorTarget;
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
    * @param data
    */
   virtual void render( Renderer& renderer, const LightingRenderData& data ) {}

   /**
    * Tells if the light is set to cast shadows.
    */
   inline bool castsShadows() const { return m_castsShadows; }
};

///////////////////////////////////////////////////////////////////////////////

class LightUtils
{
public:
   static void calculateHalfPixel( Renderer& renderer, ShaderTexture* inputTex, Vector& halfPixel );
};

///////////////////////////////////////////////////////////////////////////////
