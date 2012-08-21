/// @file   core-Renderer\DirectionalLight.h
/// @brief  a directional light
#ifndef _DIRECTIONAL_LIGHT_H
#define _DIRECTIONAL_LIGHT_H

#include "core-Renderer\Light.h"
#include "core\Color.h"
#include "core\UniqueObject.h"
#include "core\Array.h"
#include "core\AABoundingBox.h"
#include "core-Renderer\VertexShaderConfigurator.h"
#include "core-Renderer\Viewport.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class Geometry;
class RenderTarget;
class RenderingView;
struct AABoundingBox;
class Camera;

///////////////////////////////////////////////////////////////////////////////

#define NUM_CASCADES       4
#define CASCADES_IN_ROW    2

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a special kind of node that represents a directional light in a scene
 */
class DirectionalLight : public Light, public UniqueObject< DirectionalLight >
{
   DECLARE_CLASS();

private:
   struct CascadeConfig
   {
      Viewport             m_viewport;
      AABoundingBox        m_lightFrustumBounds;
      float                m_cameraNearZ;
      float                m_cameraFarZ;
   };

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

public:
   // static data
   Color                m_color;
   float                m_strength;

   /// runtime data
   PixelShader*         m_lightingShader;
   PixelShader*         m_shadowDepthMapShader;
   PixelShader*         m_shadowProjectionPS;

   Array< Geometry* >   m_visibleGeometry;
   CascadeConfig        m_cascadeConfigs[NUM_CASCADES];

public:
   DirectionalLight( const std::string& name = "" );
   virtual ~DirectionalLight();

   // -------------------------------------------------------------------------
   // Light implementation
   // -------------------------------------------------------------------------
   void render( Renderer& renderer, const LightingRenderData& data );

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

private:
   void initialize();

   // -------------------------------------------------------------------------
   // Lighting computation related methods
   // -------------------------------------------------------------------------
   void renderLighting( Renderer& renderer, const LightingRenderData& data, bool drawShadows );

   // -------------------------------------------------------------------------
   // Shadow map computation related methods
   // -------------------------------------------------------------------------
   void renderShadowMap( Renderer& renderer, const LightingRenderData& data );

   void calculateCascadesBounds( Camera& activeCamera, float pcfBlurSize, float cascadeDimensions, const RenderingView* renderingView );

   void calculateCameraCascadeFrustumBounds( Camera& activeCamera, float intervalBegin, float intervalEnd, AABoundingBox& outFrustumPart ) const;

   void createBBPoints( const AABoundingBox& inAABB, Vector* outBBPoints ) const;

   void calculateLightClippingPlanes( const Vector* sceneBBInLightSpace, AABoundingBox& inOutLightFrustumBounds ) const;

   void renderCascades( Renderer& renderer, Camera& activeCamera, Camera& lightCamera, const LightingRenderData& data );

   void combineCascades( Renderer& renderer, const LightingRenderData& data, Camera& activeCamera, Camera& lightCamera, float cascadeDimensions );
};

///////////////////////////////////////////////////////////////////////////////

#endif // _DIRECTIONAL_LIGHT_H
