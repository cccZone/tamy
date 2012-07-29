#include "core-Renderer\DirectionalLight.h"
#include "core\BoundingSpace.h"
#include "core\ResourcesManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Defines.h"
#include "core-Renderer\FullscreenQuad.h"
#include "core-Renderer\BasicRenderCommands.h"

// TODO: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 1. better debug representation of the directional light ( an arrow or sth )
// 2. bump mapping

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( DirectionalLight );
   PARENT( Light );
   PROPERTY_EDIT( "Color", Color, m_color );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

DirectionalLight::DirectionalLight( const std::string& name )
   : Light( name )
   , m_color(1, 1, 1, 1 )
   , m_pixelShader( NULL )
{
   setBoundingVolume( new BoundingSpace() ); 
   initialize();
}

///////////////////////////////////////////////////////////////////////////////

DirectionalLight::~DirectionalLight()
{
   // resources manager will take care of these objects in due time
   m_pixelShader = NULL;
}


///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::onObjectLoaded()
{
   __super::onObjectLoaded();

   initialize();
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::render( Renderer& renderer, ShaderTexture* depthNormalsTex, ShaderTexture* sceneColorTex )
{
   if ( !m_pixelShader )
   {
      return;
   }
   
   const Matrix& globalMtx = getGlobalMtx();

   // set and configure the pixel shader
   RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_pixelShader );
   {
      psComm->setVec4( "g_lightDirWS", globalMtx.forwardVec() );
      psComm->setVec4( "g_lightColor", (const Vector&)m_color );

      psComm->setTexture( "g_DepthNormals", *depthNormalsTex );
      psComm->setTexture( "g_SceneColor", *sceneColorTex );
   }

   // draw the geometry
   uint quadWidth = renderer.getViewportWidth();
   uint quadHeight = renderer.getViewportHeight();
   new ( renderer() ) RCFullscreenQuad( quadWidth, quadHeight );

   // cleanup
   new ( renderer() ) RCUnbindPixelShader( *m_pixelShader );

}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::initialize()
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   FilePath psPath( LIGHTING_SHADERS_DIR "directionalLight.tpsh" );
   m_pixelShader = resMgr.create< PixelShader >( psPath, true );
}

///////////////////////////////////////////////////////////////////////////////
