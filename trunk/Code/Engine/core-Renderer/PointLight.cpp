#include "core-Renderer\PointLight.h"
#include "core\BoundingSphere.h"
#include "core\ResourcesManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Defines.h"
#include "core-Renderer\FullscreenQuad.h"
#include "core-Renderer\BasicRenderCommands.h"
#include "core-Renderer\TriangleMesh.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( PointLight );
   PARENT( Light );
   PROPERTY_EDIT( "Color", Color, m_color );
   PROPERTY_EDIT( "strength", float, m_strength );
   PROPERTY_EDIT( "attenuation", float, m_attenuation );
   PROPERTY_EDIT( "radius", float, m_radius );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

PointLight::PointLight( const std::string& name )
   : Light( name )
   , m_color(1, 1, 1, 1 )
   , m_attenuation( 1.0f )
   , m_radius( 1.0f )
   , m_strength( 1.0f )
   , m_pixelShader( NULL )
   , m_vertexShader( NULL )
   , m_pointLightMesh( NULL )
   , m_boundingSphere( NULL )
{
   initialize();
}

///////////////////////////////////////////////////////////////////////////////

PointLight::~PointLight()
{
   // resources manager will take care of these objects in due time
   m_pixelShader = NULL;
   m_vertexShader = NULL;
   m_pointLightMesh = NULL;
}


///////////////////////////////////////////////////////////////////////////////

void PointLight::onObjectLoaded()
{
   __super::onObjectLoaded();

   initialize();
}

///////////////////////////////////////////////////////////////////////////////

void PointLight::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_radius" )
   {
      // we don't use scaling for entity transforms, that's why we need to manually
      // change the bounding sphere radius
      m_boundingSphere->radius = m_radius;
   }
}

///////////////////////////////////////////////////////////////////////////////

void PointLight::render( Renderer& renderer, ShaderTexture* depthNormalsTex, ShaderTexture* sceneColorTex )
{
   if ( !m_vertexShader || !m_pixelShader || !m_pointLightMesh )
   {
      return;
   }

   Camera& activeCamera = renderer.getActiveCamera();
   Matrix globalMtx = getGlobalMtx();

   Matrix viewProjMtx;
   viewProjMtx.setMul( activeCamera.getViewMtx(), activeCamera.getProjectionMtx() ); 

   // set and configure the pixel shader
   RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_pixelShader );
   {
      Matrix mtxInvProj;
      mtxInvProj.setInverse( activeCamera.getProjectionMtx() );

      Vector lightOriginViewSpace;
      activeCamera.getViewMtx().transform( globalMtx.position(), lightOriginViewSpace );

      psComm->setVec4( "g_lightOriginVS", lightOriginViewSpace );
      psComm->setVec4( "g_lightColor", ( const Vector& )m_color );
      psComm->setFloat( "g_strength", m_strength );
      psComm->setFloat( "g_attenuation", m_attenuation );
      psComm->setFloat( "g_radius", m_radius );
      psComm->setFloat( "g_farZ", activeCamera.getFarClippingPlane() );
      psComm->setMtx( "g_mtxProjToView", mtxInvProj );
      psComm->setTexture( "g_DepthNormals", *depthNormalsTex );
      psComm->setTexture( "g_SceneColor", *sceneColorTex );
   }

   // set and configure the vertex shader
   RCBindVertexShader* vsComm = new ( renderer() ) RCBindVertexShader( *m_vertexShader );
   {
      Matrix scaleMtx; scaleMtx.scaleUniform( m_radius );
      Matrix modelViewProjMtx;      
      modelViewProjMtx.setMul( scaleMtx, globalMtx ).mul( viewProjMtx );

      vsComm->setMtx( "g_mtxModelViewProj", modelViewProjMtx );
   }

   // draw the geometry
   m_pointLightMesh->render( renderer );

   // cleanup
   new ( renderer() ) RCUnbindVertexShader( *m_vertexShader );
   new ( renderer() ) RCUnbindPixelShader( *m_pixelShader );

}

///////////////////////////////////////////////////////////////////////////////

void PointLight::initialize()
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   FilePath vsPath( LIGHTING_SHADERS_DIR "pointLight.tvsh" );
   m_vertexShader = resMgr.create< VertexShader >( vsPath, true );

   FilePath psPath( LIGHTING_SHADERS_DIR "pointLight.tpsh" );
   m_pixelShader = resMgr.create< PixelShader >( psPath, true );

   FilePath meshPath( LIGHTING_SHADERS_DIR "pointLight.ttm" );
   m_pointLightMesh = resMgr.create< TriangleMesh >( meshPath, true );

   if ( !m_boundingSphere )
   {
      m_boundingSphere = new BoundingSphere( Vector::ZERO, m_radius );
      setBoundingVolume( m_boundingSphere ); 
   }
   else
   {
      m_boundingSphere->radius = m_radius;
   }
}

///////////////////////////////////////////////////////////////////////////////
