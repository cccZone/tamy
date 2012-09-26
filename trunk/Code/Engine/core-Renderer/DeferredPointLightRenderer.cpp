#include "core-Renderer\DeferredPointLightRenderer.h"
#include "core\ResourcesManager.h"
#include "core-Renderer\DeferredLightingRenderData.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Defines.h"
#include "core-Renderer\FullscreenQuad.h"
#include "core-Renderer\BasicRenderCommands.h"
#include "core-Renderer\TriangleMesh.h"
#include "core-Renderer\RenderTarget.h"
#include "core-Renderer\PointLight.h"
#include "core-Renderer\ShaderUtils.h"


///////////////////////////////////////////////////////////////////////////////

DeferredPointLightRenderer::DeferredPointLightRenderer()
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   FilePath vsPath( LIGHTING_SHADERS_DIR "Deferred/Point/pointLight.tvsh" );
   m_vertexShader = resMgr.create< VertexShader >( vsPath, true );

   FilePath psPath( LIGHTING_SHADERS_DIR "Deferred/Point/pointLight.tpsh" );
   m_pixelShader = resMgr.create< PixelShader >( psPath, true );

   FilePath meshPath( LIGHTING_SHADERS_DIR "Deferred/Point/pointLight.ttm" );
   m_pointLightMesh = resMgr.create< TriangleMesh >( meshPath, true );
}

///////////////////////////////////////////////////////////////////////////////

DeferredPointLightRenderer::~DeferredPointLightRenderer()
{
   m_vertexShader = NULL;
   m_pixelShader = NULL;
   m_pointLightMesh = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DeferredPointLightRenderer::render( Renderer& renderer, const PointLight* light, const DeferredLightingRenderData& data )
{
   if ( !m_pixelShader || !m_vertexShader || !m_pointLightMesh )
   {
      return;
   }

   Camera& activeCamera = renderer.getActiveCamera();
   Matrix globalMtx = light->getGlobalMtx();

   Matrix viewProjMtx;
   viewProjMtx.setMul( activeCamera.getViewMtx(), activeCamera.getProjectionMtx() ); 

   // activate the final render target
   new ( renderer() ) RCActivateRenderTarget( data.m_finalLightColorTarget );

   // set and configure the pixel shader
   RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_pixelShader, renderer );
   {
      Matrix mtxInvProj;
      mtxInvProj.setInverse( activeCamera.getProjectionMtx() );

      Vector lightOriginViewSpace;
      activeCamera.getViewMtx().transform( globalMtx.position(), lightOriginViewSpace );

      Vector halfPixel;
      ShaderUtils::calculateHalfPixel( renderer, data.m_depthTex, halfPixel );

      psComm->setVec4( "g_halfPixel", halfPixel );
      psComm->setVec4( "g_lightOriginVS", lightOriginViewSpace );
      psComm->setVec4( "g_lightColor", ( const Vector& )light->m_color );
      psComm->setFloat( "g_strength", light->m_strength );
      psComm->setFloat( "g_attenuation", light->m_attenuation );
      psComm->setFloat( "g_radius", light->m_radius );
      psComm->setFloat( "g_farZ", activeCamera.getFarClippingPlane() );
      psComm->setMtx( "g_mtxProjToView", mtxInvProj );
      psComm->setTexture( "g_Depth", data.m_depthTex );
      psComm->setTexture( "g_Normals", data.m_normalsTex );
      psComm->setTexture( "g_Specular", data.m_specularTex );
      psComm->setTexture( "g_SceneColor", data.m_sceneColorTex );
      psComm->setInt( "g_materialsTexSize", data.m_materialsDescriptorsTex->getWidth() );
      psComm->setTexture( "g_MaterialIndices", data.m_materialIndicesTex );
      psComm->setTexture( "g_MaterialsDescr", data.m_materialsDescriptorsTex );
   }

   // set and configure the vertex shader
   RCBindVertexShader* vsComm = new ( renderer() ) RCBindVertexShader( *m_vertexShader, renderer );
   {
      Matrix scaleMtx; scaleMtx.scaleUniform( light->m_radius );
      Matrix modelViewProjMtx;      
      modelViewProjMtx.setMul( scaleMtx, globalMtx ).mul( viewProjMtx );

      vsComm->setMtx( "g_mtxModelViewProj", modelViewProjMtx );
   }

   // draw the geometry
   m_pointLightMesh->render( renderer );

   // cleanup
   new ( renderer() ) RCUnbindVertexShader( *m_vertexShader );
   new ( renderer() ) RCUnbindPixelShader( *m_pixelShader, renderer );
   new ( renderer() ) RCDeactivateRenderTarget();
}

///////////////////////////////////////////////////////////////////////////////
