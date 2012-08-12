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
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\VertexShaderConfigurator.h"


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
         position.setMul( mtx.forwardVec(), -500 );// TODO: !!! light distance needs to be calculated accurately, or else those nasty steps artifacts will be visible
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
      new ( renderer() ) RCBindPixelShader( *m_shadowDepthMapShader, renderer );

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

      new ( renderer() ) RCUnbindPixelShader( *m_shadowDepthMapShader, renderer );

      // no need to deactivate current render target, as we'll be reassigning it in a sec
   }

   // 3. again, clean the depth buffer
   new ( renderer() ) RCClearDepthBuffer();

   // 4. render the shadow projection map
   {
      new ( renderer() ) RCActivateRenderTarget( screenSpaceShadowMap );
      RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_shadowProjectionPS, renderer );
      {
         // set the shadow map
         psComm->setFloat( "g_texelWidth", 1.0f / (float)shadowDepthBuffer->getWidth() );
         psComm->setFloat( "g_texelHeight", 1.0f / (float)shadowDepthBuffer->getHeight() );
         psComm->setTexture( "g_shadowDepthMap", *shadowDepthBuffer );
      }
      

      // TODO: !!!!!!!!!! DOCUMENT this configurator - it's a tool that allows to set custom values on a vertex shader depending 
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
      // So we're building a veretx shader family tree this way - teh default technique being responsible for rendering the geometry
      // itself, and additional techniques are built on top of that and provide additional data ( such as data to compute shadows ).
      struct VSSetter : public VertexShaderConfigurator
      {
         Matrix      m_lightViewProjMtx;
         Matrix      m_textureMtx;

         VSSetter( Camera& activeCamera, Camera& lightCamera, RenderTarget* shadowDepthBuffer )
         {
            // set the matrix used to calculate the shadow map texel position
            m_lightViewProjMtx.setMul( lightCamera.getViewMtx(), lightCamera.getProjectionMtx() );

            float widthTexOffs  = 0.5f + (0.5f / (float)shadowDepthBuffer->getWidth() );
            float heightTexOffs = 0.5f + (0.5f / (float)shadowDepthBuffer->getHeight() );
            Matrix matTexAdj( 0.5f,                   0.0f,    0.0f,    0.0f,
                              0.0f,                  -0.5f,    0.0f,    0.0f,
                              0.0f,                   0.0f,    1.0f,    0.0f,
                              widthTexOffs,  heightTexOffs,    0.0f,    1.0f );

            m_textureMtx.setMul( m_lightViewProjMtx, matTexAdj );
         }

         void configure( const Geometry& geometry, RCBindVertexShader* command )
         {
            const Matrix& geometryWorldMtx = geometry.getGlobalMtx();

            Matrix lightViewProjMtx;
            lightViewProjMtx.setMul( geometryWorldMtx, m_lightViewProjMtx );

            Matrix textureMtx;
            textureMtx.setMul( geometryWorldMtx, m_textureMtx );

            command->setMtx( "g_matLightViewProj", lightViewProjMtx );
            command->setMtx( "g_matTexture", textureMtx );
         }
      } vsSetter( activeCamera, lightCamera, shadowDepthBuffer );

      // render visible scene elements
      uint sceneElemsCount = geometryToRender.size();
      for ( uint i = 0; i < sceneElemsCount; ++i )
      {
         geometryToRender[i]->render( renderer, &vsSetter );
      }

      new ( renderer() ) RCUnbindPixelShader( *m_shadowProjectionPS, renderer );
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
