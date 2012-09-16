#include "core-Renderer\AmbientLight.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\BasicRenderCommands.h"
#include "core-Renderer\RenderTarget.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\FullscreenQuad.h"
#include "core-Renderer\Defines.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( AmbientLight );
   PARENT( SpatialEntity );
   PROPERTY_EDIT( "color", Color, m_lightColor );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

AmbientLight::AmbientLight( const std::string& name )
   : SpatialEntity( name )
   , m_lightColor( 0.3f, 0.3f, 0.3f, 1.0f )
   , m_shader( NULL )
{
   initialize();
}

///////////////////////////////////////////////////////////////////////////////

AmbientLight::~AmbientLight()
{
   m_shader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void AmbientLight::onObjectLoaded()
{
   __super::onObjectLoaded();
   initialize();
}

///////////////////////////////////////////////////////////////////////////////

void AmbientLight::render( Renderer& renderer, ShaderTexture* sceneColorTex, RenderTarget* renderTarget )
{
   if ( !m_shader )
   {
      return;
   }

   // activate the final render target
   new ( renderer() ) RCActivateRenderTarget( renderTarget );

   // set and configure the pixel shader
   RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_shader, renderer );
   {
      const Matrix& globalMtx = getGlobalMtx();

      Vector halfPixel;
      LightUtils::calculateHalfPixel( renderer, sceneColorTex, halfPixel );

      psComm->setVec4( "g_halfPixel", halfPixel );
      psComm->setVec4( "g_lightColor", (const Vector&)m_lightColor );
      psComm->setTexture( "g_SceneColor", sceneColorTex );
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

void AmbientLight::initialize()
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   FilePath psPath( WORLD_SHADERS_DIR "ambientLight.tpsh" );
   m_shader = resMgr.create< PixelShader >( psPath, true );
}

///////////////////////////////////////////////////////////////////////////////
