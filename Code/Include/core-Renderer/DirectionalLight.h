/// @file   core-Renderer\DirectionalLight.h
/// @brief  a directional light
#ifndef _DIRECTIONAL_LIGHT_H
#define _DIRECTIONAL_LIGHT_H

#include "core-Renderer\Light.h"
#include "core\Color.h"
#include "core\UniqueObject.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class Geometry;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a special kind of node that represents a directional light in a scene
 */
class DirectionalLight : public Light, public UniqueObject< DirectionalLight >
{
   DECLARE_CLASS();

public:
   // static data
   Color                m_color;
   float                m_strength;

   /// runtime data
   PixelShader*         m_lightingShader;
   PixelShader*         m_shadowDepthMapShader;
   PixelShader*         m_shadowProjectionShader;

   Array< Geometry* >   m_visibleGeometry;

public:
   DirectionalLight( const std::string& name = "" );
   virtual ~DirectionalLight();

   // -------------------------------------------------------------------------
   // Light implementation
   // -------------------------------------------------------------------------
   void renderLighting( Renderer& renderer, ShaderTexture* depthNormalsTex, ShaderTexture* sceneColorTex );
   void renderShadowMap( Renderer& renderer, RenderTarget* shadowDepthBuffer, RenderTarget* screenSpaceShadowMap, const RenderingView* renderedSceneView );

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

private:
   void initialize();
};

///////////////////////////////////////////////////////////////////////////////

#endif // _DIRECTIONAL_LIGHT_H
