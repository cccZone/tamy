#include "core-Renderer\SingleTextureEffect.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\EffectShader.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\Renderer.h"
#include "core-MVC\SpatialEntity.h"
#include "core-Renderer\Defines.h"
#include "core-MVC.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( SingleTextureEffect, Entity )
   PROPERTY_EDIT( "material", Material, m_material )
   PROPERTY_EDIT( "texture", Texture*, m_texture )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

SingleTextureEffect::SingleTextureEffect()
   : m_parentNode( NULL )
   , m_effect( NULL )
   , m_texture( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::setMaterial( const Material& material )
{
   m_material = material;
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::setTexture( Texture& texture )
{
   m_texture = &texture;
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onPreRender( Renderer& renderer )
{
   if ( !m_parentNode || !m_effect )
   {
      return;
   }

   Camera& camera = renderer.getActiveCamera();
   RCBindEffect* comm = new ( renderer() ) RCBindEffect( *m_effect );

   D3DXMATRIX worldViewMtx = m_parentNode->getGlobalMtx() * camera.getViewMtx();
   comm->setMtx( "g_mWorldView", worldViewMtx );
   comm->setMtx( "g_mProjection", camera.getProjectionMtx() );

   comm->setVec4( "g_MaterialAmbientColor", ( D3DXVECTOR4 )m_material.getAmbientColor() );
   comm->setVec4( "g_MaterialDiffuseColor", ( D3DXVECTOR4 )m_material.getDiffuseColor() );

   comm->setBool( "g_UseTexture", m_texture != NULL );
   if ( m_texture != NULL )
   {
      comm->setTexture( "g_MeshTexture", *m_texture );
   }

   comm->setTechnique( "singleTextureRenderer" );
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onPostRender( Renderer& renderer )
{
   if ( !m_parentNode || !m_effect )
   {
      return;
   }

   new ( renderer() ) RCUnbindEffect( *m_effect );
}

///////////////////////////////////////////////////////////////////////////////

bool SingleTextureEffect::onEquals( const SingleTextureEffect& rhs ) const
{
   return ( m_texture == rhs.m_texture ) && ( m_material == rhs.m_material );
}

///////////////////////////////////////////////////////////////////////////////

bool SingleTextureEffect::onLess( const SingleTextureEffect& rhs ) const
{
   return ( m_texture < rhs.m_texture ) && ( &m_material < &rhs.m_material );
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onAttached( Entity& parent )
{
   Geometry* geometry = dynamic_cast< Geometry* >( &parent );
   if ( geometry )
   {
      m_parentNode = geometry;
      geometry->addState( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onDetached( Entity& parent )
{
   Geometry* geometry = dynamic_cast< Geometry* >( &parent );
   if ( geometry )
   {
      m_parentNode = NULL;
      geometry->removeState( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onComponentAdded( Component< Model >& component )
{
   ModelComponent< ResourcesManager >* comp = dynamic_cast< ModelComponent< ResourcesManager >* >( &component );
   if ( comp )
   {
      // load the shader
      ResourcesManager& rm = comp->get();
      static const char* shaderName = SHADERS_DIR "SingleTextureEffect.fx";
      m_effect = dynamic_cast< EffectShader* >( rm.findResource( shaderName ) );
      if ( !m_effect )
      {
         m_effect = new EffectShader( shaderName );
         rm.addResource( m_effect );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onObjectLoaded()
{
   __super::onObjectLoaded();

   if ( isAttached() )
   {
      m_parentNode = dynamic_cast< SpatialEntity *>( &getParent() );
   }
}

///////////////////////////////////////////////////////////////////////////////

Entity* SingleTextureEffect::cloneSelf() const
{
   SingleTextureEffect* entity = new SingleTextureEffect( *this );
   return entity;
}

///////////////////////////////////////////////////////////////////////////////
