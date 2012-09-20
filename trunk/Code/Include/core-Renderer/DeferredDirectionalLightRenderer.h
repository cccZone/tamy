/// @file   core-Renderer/DeferredDirectionalLightRenderer.h
/// @brief  utility class that renders a directional light using the deferred rendering method
#pragma once

#include "core\Array.h"
#include "core-Renderer\VertexShaderConfigurator.h"
#include "core-Renderer\CascadedShadowsUtils.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class DirectionalLight;
struct DeferredLightingRenderData;
class PixelShader;
class Geometry;

///////////////////////////////////////////////////////////////////////////////

/**
 * Utility class that renders a directional light using a deferred rendering method.
 */
class DeferredDirectionalLightRenderer
{
private:
   struct VSSetter : public VertexShaderConfigurator
   {
      Matrix               m_lightViewProjMtx;

      /**
       * Constructor.
       *
       * @param lightCamera
       */
      VSSetter( Camera& lightCamera );

      // ----------------------------------------------------------------------
      // VertexShaderConfigurator implementation
      // ----------------------------------------------------------------------
      void configure( const Geometry& geometry, RCBindVertexShader* command );
   };

   PixelShader*         m_lightingShader;
   PixelShader*         m_shadowDepthMapShader;
   PixelShader*         m_shadowProjectionPS;

   Array< Geometry* >   m_visibleGeometry;
   CascadesConfig       m_cascadeCalculationConfig;
   CascadeStage*        m_calculatedCascadeStages;

public:
   /**
    * Constructor.
    * 
    * @param cascadesCount
    * @param cascadesPerRow
    */
   DeferredDirectionalLightRenderer( uint cascadesCount, uint cascadesPerRow );
   ~DeferredDirectionalLightRenderer();

   /**
    * Renders the light
    *
    * @param renderer
    * @param light
    * @param data
    */
   void render( Renderer& renderer, const DirectionalLight* light, const DeferredLightingRenderData& data );

private:
   // -------------------------------------------------------------------------
   // Lighting computation related methods
   // -------------------------------------------------------------------------
   void renderLighting( Renderer& renderer, const DirectionalLight* light, const DeferredLightingRenderData& data, bool drawShadows );

   // -------------------------------------------------------------------------
   // Shadow map computation related methods
   // -------------------------------------------------------------------------
   void renderShadowMap( Renderer& renderer, const DirectionalLight* light, const DeferredLightingRenderData& data );

   void renderCascades( Renderer& renderer, Camera& activeCamera, Camera& lightCamera, const DeferredLightingRenderData& data );

   void combineCascades( Renderer& renderer, const DeferredLightingRenderData& data, Camera& activeCamera, Camera& lightCamera, float cascadeDimensions );

};

///////////////////////////////////////////////////////////////////////////////
