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

   const float pcfBlurSize = 3.0f;
   float cascadeDimensions = (float)( data.m_shadowDepthTexture->getWidth() ) / (float)CASCADES_IN_ROW;
   calculateCascadesBounds( activeCamera, pcfBlurSize, cascadeDimensions, data.m_renderingView );

   // set the light camera
   Camera lightCamera( "dirLightCamera", renderer, Camera::PT_ORTHO );
   const Matrix& lightGlobalMtx = getGlobalMtx();
   lightCamera.setLocalMtx( lightGlobalMtx );
   lightCamera.setPosition( Vector::ZERO );

   // render cascades
   renderCascades( renderer, activeCamera, lightCamera, data );

   // combine the cascades
   combineCascades( renderer, data, activeCamera, lightCamera, cascadeDimensions );
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::calculateCascadesBounds( Camera& activeCamera, float pcfBlurSize, float cascadeDimensions, const RenderingView* renderingView )
{
   // calculate the camera depth range
   float cameraRange = activeCamera.getFarClippingPlane() - activeCamera.getNearClippingPlane();

   // pre-calculate light camera transformations that we'll use later on during AABBs calculations
   Matrix cameraToLightViewTransform;
   {
      Matrix invViewCamera;
      invViewCamera.setInverse( activeCamera.getViewMtx() );
      Matrix lightGlobalMtx = getGlobalMtx();
      lightGlobalMtx.setPosition( Vector::ZERO );

      Matrix lightViewMtx;
      MatrixUtils::calculateViewMtx( lightGlobalMtx, lightViewMtx );

      cameraToLightViewTransform.setMul( invViewCamera, lightViewMtx );
   }

   // acquire scene's AABB and transform it to the light view space
   AABoundingBox sceneAABBInLightSpace;
   {
      const AABoundingBox& sceneBounds = renderingView->getSceneBounds();
      sceneBounds.transform( cameraToLightViewTransform, sceneAABBInLightSpace );
   }

   // calculate light frustum bounds
   AABoundingBox cascadeFrustumBounds;
   Vector worldUnitsPerTexel;
   for ( uint i = 0; i < NUM_CASCADES; ++i )
   {
      float frustumIntervalBegin = max( 1.01f, g_cascadeIntervals[i] * cameraRange );
      float frustumIntervalEnd = g_cascadeIntervals[i + 1] * cameraRange;
      m_cascadeConfigs[i].m_cameraNearZ = frustumIntervalBegin;
      m_cascadeConfigs[i].m_cameraFarZ = frustumIntervalEnd;

      calculateCascadeFrustumBounds( activeCamera, frustumIntervalBegin, frustumIntervalEnd, cascadeFrustumBounds );

      // transform the bounds to light
      AABoundingBox& lightFrustumBounds = m_cascadeConfigs[i].m_lightFrustumBounds;
      cascadeFrustumBounds.transform( cameraToLightViewTransform, lightFrustumBounds );

      // memorize the clipping planes
      float lightCameraNearZ = lightFrustumBounds.min.z;
      float lightCameraFarZ = lightFrustumBounds.max.z;

      // now adjust the light frustum a bit in order to remove the shimmering effect
      // of shadow edges whenever the camera is moved
      {
         // We calculate a looser bound based on the size of the PCF blur.  This ensures us that we're 
         // sampling within the correct map.
         float scaleDueToBlurAMT = ( (float)( pcfBlurSize * 2 + 1 ) / cascadeDimensions );
         Vector vScaleDueToBlurAMT( scaleDueToBlurAMT, scaleDueToBlurAMT, 0.0f, 0.0f );

         float normalizeByBufferSize = ( 1.0f / cascadeDimensions );
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
      // TODO:!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      lightFrustumBounds.min.z = -100.0f;
      lightFrustumBounds.max.z = 100.0f;
      
      // calculate viewport
      {
         Viewport& viewport = m_cascadeConfigs[i].m_viewport;
         viewport.m_minZ = 0.0f;
         viewport.m_maxZ = 1.0f;
         viewport.m_offsetX = (ulong)( ( i % CASCADES_IN_ROW ) * cascadeDimensions );
         viewport.m_offsetY = (ulong)( ( i / CASCADES_IN_ROW ) * cascadeDimensions );
         viewport.m_width = (ulong)cascadeDimensions;
         viewport.m_height = (ulong)cascadeDimensions;
      }
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


void DirectionalLight::renderCascades( Renderer& renderer, Camera& activeCamera, Camera& lightCamera, const ShadowRendererData& data )
{
   // store the original camera settings
   float origCameraNearZ, origCameraFarZ;
   activeCamera.getClippingPlanes( origCameraNearZ, origCameraFarZ );

   // bind the shader and set the render target
   new ( renderer() ) RCActivateRenderTarget( data.m_shadowDepthTexture );
   new ( renderer() ) RCActivateDepthBuffer( data.m_shadowDepthSurface );
   new ( renderer() ) RCBindPixelShader( *m_shadowDepthMapShader, renderer );
   new ( renderer() ) RCClearDepthBuffer();

   // render cascades
   for ( uint cascadeIdx = 0; cascadeIdx < NUM_CASCADES; ++cascadeIdx )
   {
      const AABoundingBox& cascadeBounds = m_cascadeConfigs[cascadeIdx].m_lightFrustumBounds;
      lightCamera.setNearPlaneDimensions( cascadeBounds.max.x - cascadeBounds.min.x, cascadeBounds.max.y - cascadeBounds.min.y );
      lightCamera.setClippingPlanes( cascadeBounds.min.z, cascadeBounds.max.z );
      //activeCamera.setClippingPlanes( m_cascadeConfigs[cascadeIdx].m_cameraNearZ, m_cascadeConfigs[cascadeIdx].m_cameraFarZ );

      new ( renderer() ) RCSetViewport( m_cascadeConfigs[cascadeIdx].m_viewport );

      // render the scene with the new camera
      {
         VSSetter vsSetter( lightCamera );

         m_visibleGeometry.clear();
         data.m_renderingView->collectRenderables( cascadeBounds, m_visibleGeometry );

         uint sceneElemsCount = m_visibleGeometry.size();
         for ( uint i = 0; i < sceneElemsCount; ++i )
         {
            m_visibleGeometry[i]->render( renderer, &vsSetter);
         }
      }
   }

   // clean up
   new ( renderer() ) RCResetViewport();
   new ( renderer() ) RCUnbindPixelShader( *m_shadowDepthMapShader, renderer );
   new ( renderer() ) RCDeactivateDepthBuffer( data.m_shadowDepthSurface );
   new ( renderer() ) RCDeactivateRenderTarget();

   // restore previous camera settings
   activeCamera.setClippingPlanes( origCameraNearZ, origCameraFarZ );
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::combineCascades( Renderer& renderer, const ShadowRendererData& data, Camera& activeCamera, Camera& lightCamera, float cascadeDimensions )
{
   float depthRanges[NUM_CASCADES + 1];
   Vector viewportOffsets[NUM_CASCADES];
   Matrix clipToLightSpaceMtx[NUM_CASCADES];

   float shadowMapDimension = (float)data.m_shadowDepthTexture->getWidth();

   Matrix worldToClipSpace;
   worldToClipSpace.setMul( activeCamera.getViewMtx(), activeCamera.getProjectionMtx() );

   Matrix invClipSpace;
   invClipSpace.setInverse( worldToClipSpace );

   for( uint i = 0; i < NUM_CASCADES; ++i )
   {
      CascadeConfig& config = m_cascadeConfigs[i];
      depthRanges[i] = config.m_cameraFarZ;
      viewportOffsets[i].set( (float)config.m_viewport.m_offsetX / shadowMapDimension, (float)config.m_viewport.m_offsetY / shadowMapDimension, 0 );

      // calculate the transformation matrix
      {
         const AABoundingBox& cascadeBounds = m_cascadeConfigs[i].m_lightFrustumBounds;
         lightCamera.setNearPlaneDimensions( cascadeBounds.max.x - cascadeBounds.min.x, cascadeBounds.max.y - cascadeBounds.min.y );
         lightCamera.setClippingPlanes( cascadeBounds.min.z, cascadeBounds.max.z );

         Matrix lightViewProj;
         lightViewProj.setMul( lightCamera.getViewMtx(), lightCamera.getProjectionMtx() );

         clipToLightSpaceMtx[i].setMul( invClipSpace, lightViewProj );     
      }
   }
   depthRanges[NUM_CASCADES] = FLT_MAX;

   // again, clean the depth buffer
   new ( renderer() ) RCClearDepthBuffer();

   //  render the shadow projection map
   {
      new ( renderer() ) RCActivateRenderTarget( data.m_screenSpaceShadowMap );
      RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_shadowProjectionPS, renderer );
      {
         // set the shadow map
         float texelDimension = 1.0f / cascadeDimensions;
         psComm->setFloat( "g_texelDimension", texelDimension );
         psComm->setTexture( "g_shadowDepthMap", *data.m_shadowDepthTexture );
         psComm->setFloat( "g_cascadeDepthRanges", depthRanges, NUM_CASCADES + 1 );
         psComm->setVec4( "g_cascadeOffsets", viewportOffsets, NUM_CASCADES  );
         psComm->setMtx( "g_clipToLightSpaceMtx", clipToLightSpaceMtx , NUM_CASCADES );
      }
    
      // render visible scene elements
      uint sceneElemsCount = data.m_geometryToRender->size();
      for ( uint i = 0; i < sceneElemsCount; ++i )
      {
         (*data.m_geometryToRender)[i]->render( renderer );
      }

      new ( renderer() ) RCUnbindPixelShader( *m_shadowProjectionPS, renderer );
      new ( renderer() ) RCDeactivateRenderTarget();
   }
   
   // clean the depth buffer once we're done
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
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DirectionalLight::VSSetter::VSSetter( Camera& lightCamera )
{
   m_lightViewProjMtx.setMul( lightCamera.getViewMtx(), lightCamera.getProjectionMtx() );
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::VSSetter::configure( const Geometry& geometry, RCBindVertexShader* command )
{
   const Matrix& geometryWorldMtx = geometry.getGlobalMtx();

   Matrix lightViewProjMtx;
   lightViewProjMtx.setMul( geometryWorldMtx, m_lightViewProjMtx );

   command->setMtx( "g_matLightViewProj", lightViewProjMtx );
}

///////////////////////////////////////////////////////////////////////////////
