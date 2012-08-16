#include "core-Renderer\DirectionalLight.h"
#include "core\BoundingSpace.h"
#include "core\MatrixUtils.h"
#include "core\ResourcesManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Defines.h"
#include "core-Renderer\FullscreenQuad.h"
#include "core-Renderer\BasicRenderCommands.h"
#include "core-Renderer\RenderTarget.h"
#include "core-Renderer\DepthBuffer.h"
#include "core-Renderer\RenderingView.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\VertexShaderConfigurator.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( DirectionalLight );
   PARENT( Light );
   PROPERTY_EDIT( "Color", Color, m_color );
   PROPERTY_EDIT( "strength", float, m_strength );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

#define NUM_CASCADES    4

float g_cascadeIntervals[] = { 0.0f, 5.0f / 100.0f, 15.0f / 100.0f, 60.0f / 100.0f, 1.0f };

///////////////////////////////////////////////////////////////////////////////

DirectionalLight::DirectionalLight( const std::string& name )
   : Light( name )
   , m_color(1, 1, 1, 1 )
   , m_strength( 1 )
   , m_lightingShader( NULL )
   , m_shadowDepthMapShader( NULL )
   , m_shadowProjectionPS( NULL )
{
   setBoundingVolume( new BoundingSpace() ); 
   initialize();

   m_visibleGeometry.resize( 1024, NULL );
}

///////////////////////////////////////////////////////////////////////////////

DirectionalLight::~DirectionalLight()
{
   // resources manager will take care of these objects in due time
   m_lightingShader = NULL;
   m_shadowDepthMapShader = NULL;
   m_shadowProjectionPS = NULL;
}


///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::onObjectLoaded()
{
   __super::onObjectLoaded();

   initialize();
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::renderLighting( Renderer& renderer, ShaderTexture* depthNormalsTex, ShaderTexture* sceneColorTex )
{
   if ( !m_lightingShader )
   {
      return;
   }
   
   const Matrix& globalMtx = getGlobalMtx();

   // set and configure the pixel shader
   RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_lightingShader, renderer );
   {
      Camera& activeCamera = renderer.getActiveCamera();
      Vector lightDirVS;
      activeCamera.getViewMtx().transformNorm( globalMtx.forwardVec(), lightDirVS );
      psComm->setVec4( "g_lightDirVS", lightDirVS );
      psComm->setVec4( "g_lightColor", (const Vector&)m_color );
      psComm->setFloat( "g_strength", m_strength );

      psComm->setTexture( "g_DepthNormals", *depthNormalsTex );
      psComm->setTexture( "g_SceneColor", *sceneColorTex );
   }

   // draw the geometry
   uint quadWidth = renderer.getViewportWidth();
   uint quadHeight = renderer.getViewportHeight();
   new ( renderer() ) RCFullscreenQuad( quadWidth, quadHeight );

   // cleanup
   new ( renderer() ) RCUnbindPixelShader( *m_lightingShader, renderer );
}

///////////////////////////////////////////////////////////////////////////////

// TODO: !!!!!!!!!!! shadow calculations need to be calculated along with the light propagation volume - get rid of the soft shadows node as soon as all the shadow related
// calculations work fine, and then alter the light volume shape ( the rendered one ) with respect to the cast shadows.
// We need that for situations, in which there's a point light located in another light's shadow. Such object would normally be black from the shaded side,
// but since there's a point light there, that light will influence it.
// If we just calculate a single shadow map for all lights, there's no way of telling which lights influence which regions of the scene


// TODO: !!!!!!!!!! DOCUMENT the VertexShaderConfigurator - it's a tool that allows to set custom values on a vertex shader depending 
// on the context from which that vertex shader is called ( like this for instance ).
// It works with Geometry entities, not vertex shaders themselves.
// It allows to render arbitrary geometry ( that's set up with arbitrary vertex shaders ) using arbitrary rendering technique
// ( imposed by the pixel shader in use ) by setting up the technique-related shader constants on those vertex shaders.
//
// This technology resembles DX effects, but with a difference that here the main item for us is the pixel shader.
// The pixel shader defines what sort of data it requires from the preceding vertex shader ( by setting the vertex shader technique ).
// Vertex shader on the other hand may require a specific set of constants being set for that technique - additional 
// to the constants it normally uses to render geometry using its default technique.
//
// So we're building a vertex shader family tree this way - the default technique being responsible for rendering the geometry
// itself, and additional techniques are built on top of that and provide additional data ( such as data to compute shadows ).


void DirectionalLight::renderShadowMap( Renderer& renderer, const ShadowRendererData& data )
{
   if ( !m_castsShadows || !m_shadowDepthMapShader || !m_shadowProjectionPS )
   {
      return;
   }

   Camera& activeCamera = renderer.getActiveCamera();
   const Matrix& lightGlobalMtx = getGlobalMtx();

   const float pcfBlurSize = 3.0f;

   AABoundingBox cascadesBounds[NUM_CASCADES];
   calculateCascadesBounds( activeCamera, pcfBlurSize, (float)data.m_shadowDepthTexture->getWidth(), cascadesBounds );

   // set the light camera
   Camera lightCamera( "dirLightCamera", renderer, Camera::PT_ORTHO );
   lightCamera.setLocalMtx( lightGlobalMtx );

   {
      uint cascadeIdx = 0;

      lightCamera.setNearPlaneDimensions( cascadesBounds[cascadeIdx].max.x - cascadesBounds[cascadeIdx].min.x, cascadesBounds[cascadeIdx].max.y - cascadesBounds[cascadeIdx].min.y );
      lightCamera.setClippingPlanes( cascadesBounds[cascadeIdx].min.z, cascadesBounds[cascadeIdx].max.z );
   }

   // render the cascade part
   renderCascade( renderer, activeCamera, lightCamera, data );
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::calculateCascadesBounds( Camera& activeCamera, float pcfBlurSize, float shadowMapDimensions, AABoundingBox* outArrCascadesBounds )
{
   // calculate the camera depth range
   float cameraRange = activeCamera.getFarClippingPlane() - activeCamera.getNearClippingPlane();

   Matrix invViewCamera;
   invViewCamera.setInverse( activeCamera.getViewMtx() );
   const Matrix& lightGlobalMtx = getGlobalMtx();

   Matrix lightViewMtx;
   MatrixUtils::calculateViewMtx( lightGlobalMtx, lightViewMtx );

   Matrix cameraToLightViewTransform;
   cameraToLightViewTransform.setMul( invViewCamera, lightViewMtx );

   // calculate light frustum bounds
   AABoundingBox cascadeFrustumBounds;
   Vector worldUnitsPerTexel;
   for ( uint i = 0; i < NUM_CASCADES; ++i )
   {
      float frustumIntervalBegin = max( 1.01f, g_cascadeIntervals[i] * cameraRange );
      float frustumIntervalEnd = g_cascadeIntervals[i + 1] * cameraRange;

      calculateCascadeFrustumBounds( activeCamera, frustumIntervalBegin, frustumIntervalEnd, cascadeFrustumBounds );

      // transform the bounds to light
      AABoundingBox& lightFrustumBounds = outArrCascadesBounds[i];
      cascadeFrustumBounds.transform( cameraToLightViewTransform, lightFrustumBounds );

      // memorize the clipping planes
      float lightCameraNearZ = lightFrustumBounds.min.z;
      float lightCameraFarZ = lightFrustumBounds.max.z;

      // now adjust the light frustum a bit in order to remove the shimmering effect
      // of shadow edges whenever the camera is moved
      {
         // We calculate a looser bound based on the size of the PCF blur.  This ensures us that we're 
         // sampling within the correct map.
         float scaleDueToBlurAMT = ( (float)( pcfBlurSize * 2 + 1 ) / shadowMapDimensions );
         Vector vScaleDueToBlurAMT( scaleDueToBlurAMT, scaleDueToBlurAMT, 0.0f, 0.0f );

         float normalizeByBufferSize = ( 1.0f / shadowMapDimensions );
         Vector vNormalizeByBufferSize( normalizeByBufferSize, normalizeByBufferSize, 0.0f, 0.0f );

         // We calculate the offsets as a percentage of the bound.
         Vector broaderOffset;
         broaderOffset.setSub( lightFrustumBounds.max, lightFrustumBounds.min ).mul( 0.5f ).mul( vScaleDueToBlurAMT );

         lightFrustumBounds.max.add( broaderOffset );
         lightFrustumBounds.min.sub( broaderOffset );

         // The world units per texel are used to snap the orthographic projection to texel sized increments.  
         // Because we're fitting tightly to the cascades, the shimmering shadow edges will still be present when the 
         // camera rotates.  However, when zooming in or strafing the shadow edge will not shimmer.
         worldUnitsPerTexel.setSub( lightFrustumBounds.max, lightFrustumBounds.min ).mul( vNormalizeByBufferSize );
      }

      // Snap the camera to 1-pixel increments so that camera movement doesn't cause the shadows to jitter around the edges
      {
         lightFrustumBounds.min.div( worldUnitsPerTexel );
         lightFrustumBounds.min.floor();
         lightFrustumBounds.min.mul( worldUnitsPerTexel );

         lightFrustumBounds.max.div( worldUnitsPerTexel );
         lightFrustumBounds.max.floor();
         lightFrustumBounds.max.mul( worldUnitsPerTexel );
      }

      // set the clipping planes and store the bounding box
      lightFrustumBounds.min.z = lightCameraNearZ;
      lightFrustumBounds.max.z = lightCameraFarZ;
   }

}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::calculateCascadeFrustumBounds( Camera& activeCamera, float intervalBegin, float intervalEnd, AABoundingBox& outFrustumPart ) const
{
   float prevNearZ, prevFarZ;
   activeCamera.getClippingPlanes( prevNearZ, prevFarZ );
   activeCamera.setClippingPlanes( intervalBegin, intervalEnd );

   Frustum frustum;
   activeCamera.calculateFrustum( frustum );
   frustum.calculateBoundingBox( outFrustumPart );

   activeCamera.setClippingPlanes( prevNearZ, prevFarZ );
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::renderCascade( Renderer& renderer, Camera& activeCamera, Camera& lightCamera, const ShadowRendererData& data )
{
   uint viewportWidth = (uint)activeCamera.getNearPlaneWidth();
   uint viewportHeight = (uint)activeCamera.getNearPlaneHeight();

   // 1. render the shadow depth buffer
   {
      new ( renderer() ) RCActivateRenderTarget( data.m_shadowDepthTexture );
      new ( renderer() ) RCActivateDepthBuffer( data.m_shadowDepthSurface );
      new ( renderer() ) RCClearDepthBuffer();

      new ( renderer() ) RCBindPixelShader( *m_shadowDepthMapShader, renderer );

      // render the scene with the new camera
      renderer.pushCamera( lightCamera );
      {
         m_visibleGeometry.clear();
         static BoundingSpace boundingSpace;
         data.m_renderingView->collectRenderables( boundingSpace, m_visibleGeometry );

         uint sceneElemsCount = m_visibleGeometry.size();
         for ( uint i = 0; i < sceneElemsCount; ++i )
         {
            m_visibleGeometry[i]->render( renderer );
         }
      }
      renderer.popCamera();

      new ( renderer() ) RCUnbindPixelShader( *m_shadowDepthMapShader, renderer );
      new ( renderer() ) RCDeactivateDepthBuffer( data.m_shadowDepthSurface );
   }

   // 2. again, clean the depth buffer
   new ( renderer() ) RCClearDepthBuffer();

   // 3. render the shadow projection map
   {
      new ( renderer() ) RCActivateRenderTarget( data.m_screenSpaceShadowMap );
      RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_shadowProjectionPS, renderer );
      {
         // set the shadow map
         float texelDimension = 1.0f / (float)data.m_shadowDepthTexture->getWidth();
         psComm->setFloat( "g_texelDimension", texelDimension );
         psComm->setTexture( "g_shadowDepthMap", *data.m_shadowDepthTexture );
      }
      
      struct VSSetter : public VertexShaderConfigurator
      {
         Matrix      m_lightViewProjMtx;

         VSSetter( Camera& activeCamera, Camera& lightCamera, RenderTarget* shadowDepthBuffer )
         {
            // set the matrix used to calculate the shadow map texel position
            m_lightViewProjMtx.setMul( lightCamera.getViewMtx(), lightCamera.getProjectionMtx() );
         }

         void configure( const Geometry& geometry, RCBindVertexShader* command )
         {
            const Matrix& geometryWorldMtx = geometry.getGlobalMtx();

            Matrix lightViewProjMtx;
            lightViewProjMtx.setMul( geometryWorldMtx, m_lightViewProjMtx );

            command->setMtx( "g_matLightViewProj", lightViewProjMtx );
         }
      } vsSetter( activeCamera, lightCamera, data.m_shadowDepthTexture );

      // render visible scene elements
      uint sceneElemsCount = data.m_geometryToRender->size();
      for ( uint i = 0; i < sceneElemsCount; ++i )
      {
         (*data.m_geometryToRender)[i]->render( renderer, &vsSetter );
      }

      new ( renderer() ) RCUnbindPixelShader( *m_shadowProjectionPS, renderer );
      new ( renderer() ) RCDeactivateRenderTarget();
   }
   
   // 4. clean the depth buffer once we're done
   new ( renderer() ) RCClearDepthBuffer();
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::initialize()
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   // lighting shaders
   {
      FilePath psPath( LIGHTING_SHADERS_DIR "Lights/directionalLight.tpsh" );
      m_lightingShader = resMgr.create< PixelShader >( psPath, true );
   }

   // shadow shaders
   {
      FilePath shadowDepthMapShaderPath( LIGHTING_SHADERS_DIR "Shadows/DirectionalLight/shadowDepthMapShader.tpsh" );
      m_shadowDepthMapShader = resMgr.create< PixelShader >( shadowDepthMapShaderPath, true );

      FilePath shadowProjectionPSPath( LIGHTING_SHADERS_DIR "Shadows/DirectionalLight/shadowProjectionShader.tpsh" );
      m_shadowProjectionPS = resMgr.create< PixelShader >( shadowProjectionPSPath, true );
   }
}

///////////////////////////////////////////////////////////////////////////////
