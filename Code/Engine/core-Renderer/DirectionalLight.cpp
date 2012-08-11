#include "core-Renderer\DirectionalLight.h"
#include "core\BoundingSpace.h"
#include "core\ResourcesManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Defines.h"
#include "core-Renderer\FullscreenQuad.h"
#include "core-Renderer\BasicRenderCommands.h"
#include "core-Renderer\RenderTarget.h"
#include "core-Renderer\RenderingView.h"
#include "core-Renderer\Geometry.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( DirectionalLight );
   PARENT( Light );
   PROPERTY_EDIT( "Color", Color, m_color );
   PROPERTY_EDIT( "strength", float, m_strength );
END_OBJECT();

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
   RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_lightingShader );
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
   new ( renderer() ) RCUnbindPixelShader( *m_lightingShader );
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::renderShadowMap( Renderer& renderer, RenderTarget* shadowDepthBuffer, RenderTarget* screenSpaceShadowMap, const RenderingView* renderedSceneView, const Array< Geometry* >& geometryToRender )
{
   if ( !m_castsShadows || !m_shadowDepthMapShader || !m_shadowProjectionPS )
   {
      return;
   }

   Camera& activeCamera = renderer.getActiveCamera();
   uint viewportWidth = (uint)activeCamera.getNearPlaneWidth();
   uint viewportHeight = (uint)activeCamera.getNearPlaneHeight();

   // use light as a camera
   Camera lightCamera( "dirLightCamera", renderer, Camera::PT_PERSPECTIVE );
   {
      float farZ = activeCamera.getFarClippingPlane();
      lightCamera.setClippingPlanes( activeCamera.getNearClippingPlane(), activeCamera.getFarClippingPlane() );
      lightCamera.setNearPlaneDimensions( viewportWidth, viewportHeight );

      Matrix mtx = getGlobalMtx();
      {
         // move the camera to the very top of the scene's bounding box, so that all elements are captured
         Vector position;
         position.setMul( mtx.forwardVec(), -30 );
         mtx.setPosition( position );
         lightCamera.setLocalMtx( mtx );
      }
      // TODO: !!!!!!! - once the shadows work fine, optimize the number of rendered shadows by not taking the geometry that can't be possibly seen by the active camera into account ( A BSP test or something )
   }

   // 1. we need to clear the depth buffer before this operation
   new ( renderer() ) RCClearDepthBuffer();

   // 2. render the shadow depth buffer
   {
      new ( renderer() ) RCActivateRenderTarget( shadowDepthBuffer );
      new ( renderer() ) RCBindPixelShader( *m_shadowDepthMapShader );

      // render the scene with the new camera
      renderer.pushCamera( lightCamera );
      {
         m_visibleGeometry.clear();
         static BoundingSpace boundingSpace;
         renderedSceneView->collectRenderables( boundingSpace, m_visibleGeometry );

         uint sceneElemsCount = m_visibleGeometry.size();
         for ( uint i = 0; i < sceneElemsCount; ++i )
         {
            m_visibleGeometry[i]->render( renderer );
         }
      }
      renderer.popCamera();

      new ( renderer() ) RCUnbindPixelShader( *m_shadowDepthMapShader );

      // no need to deactivate current render target, as we'll be reassigning it in a sec
   }
   
   // TODO: !!!!!!!!!!!!!!! I need to be able to render any geometry using a custom vertex shader.
   // But since we have different types of geometry ( static, skinned - with different inputs etc. )
   // I need to be able to customize the vertex shader accordingly

   // 3. again, clean the depth buffer
   new ( renderer() ) RCClearDepthBuffer();

   // 4. render the shadow projection map
   {
      new ( renderer() ) RCActivateRenderTarget( screenSpaceShadowMap );
      RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_shadowProjectionPS );
      {
         // set the shadow map
         psComm->setTexture( "g_shadowDepthMap", *shadowDepthBuffer );

         {
            const Matrix& camView = activeCamera.getViewMtx(); 
            Matrix invCamProj;
            invCamProj.setInverse( activeCamera.getProjectionMtx() );

            // set the light position
            Vector lightPos, lightPosVS, lightForward, lightForwardVS;
            lightCamera.getPosition( lightPos );
            lightCamera.getLookVec( lightForward );
            camView.transform( lightPos, lightPosVS );
            camView.transformNorm( lightForward, lightForwardVS );
            psComm->setVec4( "g_lightPosVS", lightPosVS );
            psComm->setVec4( "g_lightForwardVS", lightForwardVS );
            psComm->setMtx( "g_invProj", invCamProj );
         }

         // set the matrix used to calculate the shadow map texel position
         {
            Matrix invCamViewProj;
            invCamViewProj.setMul( activeCamera.getViewMtx(), activeCamera.getProjectionMtx() ).invert();

            Matrix lightViewProjMtx;
            lightViewProjMtx.setMul( lightCamera.getViewMtx(), lightCamera.getProjectionMtx() );

            Matrix camViewProjTolightViewProjMtx;
            camViewProjTolightViewProjMtx.setMul( invCamViewProj, lightViewProjMtx );

            float widthTexOffs  = 0.5 + (0.5 / (float)shadowDepthBuffer->getWidth() );
            float heightTexOffs = 0.5 + (0.5 / (float)shadowDepthBuffer->getHeight() );
            Matrix matTexAdj( 0.5f,                   0.0f,    0.0f,    0.0f,
                              0.0f,                  -0.5f,    0.0f,    0.0f,
                              0.0f,                   0.0f,    1.0f,    0.0f,
                              widthTexOffs,  heightTexOffs,    0.0f,    1.0f );

            Matrix textureMtx;
            textureMtx.setMul( camViewProjTolightViewProjMtx, matTexAdj );
            psComm->setMtx( "g_textureMtx", textureMtx );
         }
      }

      // render visible scene elements
      uint sceneElemsCount = geometryToRender.size();
      for ( uint i = 0; i < sceneElemsCount; ++i )
      {
         geometryToRender[i]->render( renderer );
      }

      new ( renderer() ) RCUnbindPixelShader( *m_shadowProjectionPS );
      new ( renderer() ) RCDeactivateRenderTarget();
   }
   
   // 5. clean the depth buffer once we're done
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
