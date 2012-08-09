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
   , m_shadowProjectionShader( NULL )
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
   m_shadowProjectionShader = NULL;
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

void DirectionalLight::renderShadowMap( Renderer& renderer, RenderTarget* shadowDepthBuffer, RenderTarget* screenSpaceShadowMap, const RenderingView* renderedSceneView )
{
   if ( !m_castsShadows || !m_shadowDepthMapShader || !m_shadowProjectionShader )
   {
      return;
   }

   Camera& activeCamera = renderer.getActiveCamera();
   uint viewportWidth = (uint)activeCamera.getNearPlaneWidth();
   uint viewportHeight = (uint)activeCamera.getNearPlaneHeight();


   // 1. we need to clear the depth buffer before this operation
   new ( renderer() ) RCClearDepthBuffer();

   // 2. render the shadow depth buffer
   {
      new ( renderer() ) RCActivateRenderTarget( shadowDepthBuffer );
      new ( renderer() ) RCBindPixelShader( *m_shadowDepthMapShader );

      // use light as a camera
      Camera camera( "dirLightCamera", renderer, Camera::PT_ORTHO );
      {
         camera.setClippingPlanes( activeCamera.getNearClippingPlane(), activeCamera.getFarClippingPlane() );
         camera.setNearPlaneDimensions( viewportWidth, viewportHeight );
         camera.setLocalMtx( getGlobalMtx() );
         // TODO: !!!!!!! - move the camera to the very top of the scene's bounding box, so that all elements are captured,
         // - once the shadows work fine, optimize the number of rendered shadows by not taking the geometry that can't be possibly seen by the active camera into account ( A BSP test or something )
      }

      // render the scene with the new camera
      renderer.pushCamera( camera );
      {
         m_visibleGeometry.clear();
         const Frustum& frustum = camera.getFrustum();
         renderedSceneView->collectRenderables( frustum, m_visibleGeometry );

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

   // 3. render the projected shadow map
   {
      new ( renderer() ) RCActivateRenderTarget( screenSpaceShadowMap );
      RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_shadowProjectionShader );
      {
         psComm->setTexture( "g_shadowDepthMap", *shadowDepthBuffer );
      }

      new ( renderer() ) RCFullscreenQuad( viewportWidth, viewportHeight );

      new ( renderer() ) RCUnbindPixelShader( *m_shadowProjectionShader );
      new ( renderer() ) RCDeactivateRenderTarget();
   }
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

      FilePath shadowProjectionShaderPath( LIGHTING_SHADERS_DIR "Lights/DirectionalLight/shadowProjectionShader.tpsh" );
      m_shadowProjectionShader = resMgr.create< PixelShader >( shadowProjectionShaderPath, true );
   }
}

///////////////////////////////////////////////////////////////////////////////
