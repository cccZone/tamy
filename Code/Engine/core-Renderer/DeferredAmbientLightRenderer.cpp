#include "core-Renderer\DeferredAmbientLightRenderer.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\BasicRenderCommands.h"
#include "core-Renderer\RenderTarget.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\FullscreenQuad.h"
#include "core-Renderer\Defines.h"
#include "core-Renderer\ShaderUtils.h"
#include "core-Renderer\AmbientLight.h"
#include "core-Renderer\Defines.h"
#include "core-Renderer\DeferredLightingRenderData.h"
#include "core-Renderer\MaterialsDB.h"
#include "core\ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

DeferredAmbientLightRenderer::DeferredAmbientLightRenderer()
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   FilePath psPath( LIGHTING_SHADERS_DIR "Deferred/Ambient/ambientLight.tpsh" );
   m_shader = resMgr.create< PixelShader >( psPath, true );
}

///////////////////////////////////////////////////////////////////////////////

DeferredAmbientLightRenderer::~DeferredAmbientLightRenderer()
{
   m_shader = NULL;
}


///////////////////////////////////////////////////////////////////////////////

void DeferredAmbientLightRenderer::render( Renderer& renderer, const AmbientLight* light, const DeferredLightingRenderData& data )
{
   if ( !m_shader )
   {
      return;
   }

   // activate the final render target
   new ( renderer() ) RCActivateRenderTarget( data.m_finalLightColorTarget );

   // set and configure the pixel shader
   RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_shader, renderer );
   {
      Vector halfPixel;
      ShaderUtils::calculateHalfPixel( renderer, data.m_sceneColorTex, halfPixel );

      psComm->setVec4( "g_halfPixel", halfPixel );
      psComm->setVec4( "g_lightColor", (const Vector&)light->m_lightColor );
      psComm->setTexture( "g_SceneColor", data.m_sceneColorTex );
      psComm->setInt( "g_materialsTexSize", data.m_materialsDescriptorsTex->getWidth() );
      psComm->setTexture( "g_MaterialIndices", data.m_materialIndicesTex );
      psComm->setTexture( "g_MaterialsDescr", data.m_materialsDescriptorsTex );
   }

   // draw the geometry
   uint quadWidth = renderer.getViewportWidth();
   uint quadHeight = renderer.getViewportHeight();
   new ( renderer() ) RCFullscreenQuad( quadWidth, quadHeight );

   // cleanup
   new ( renderer() ) RCUnbindPixelShader( *m_shader, renderer );
   new ( renderer() ) RCDeactivateRenderTarget();
}

///////////////////////////////////////////////////////////////////////////////
