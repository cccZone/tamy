#include "core-Renderer\SingleTextureEffect.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\EffectShader.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\SpatialEntity.h"
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
   , m_camera( NULL )
   , m_texture( NULL )
   , m_geometry( NULL )
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

void SingleTextureEffect::setGeometry( GeometryResource& geometry )
{
   m_geometry = &geometry;
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::render()
{
   if ( !m_parentNode || !m_camera || !m_effect || !m_geometry )
   {
      return;
   }

   D3DXMATRIX worldViewMtx = m_parentNode->getGlobalMtx() * m_camera->getViewMtx();
   m_effect->setMtx( "g_mWorldView", worldViewMtx );
   m_effect->setMtx( "g_mProjection", m_camera->getProjectionMtx() );

   m_effect->setVec4( "g_MaterialAmbientColor", ( D3DXVECTOR4 )m_material.getAmbientColor() );
   m_effect->setVec4( "g_MaterialDiffuseColor", ( D3DXVECTOR4 )m_material.getDiffuseColor() );

   m_effect->setBool( "g_UseTexture", m_texture != NULL );
   if ( m_texture != NULL )
   {
      m_effect->setTexture( "g_MeshTexture", *m_texture );
   }

   m_effect->setTechnique( "singleTextureRenderer" );

   m_effect->render( *m_geometry );
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onAttached( Entity& parent )
{
   m_parentNode = dynamic_cast< SpatialEntity *>( &parent );
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onDetached(Entity& parent)
{
   m_parentNode = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onAttached( Model& hostModel ) 
{
   m_camera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onDetached( Model& hostModel ) 
{
   m_camera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onComponentAdded( Component< Model >& component )
{
   {
      ModelComponent< Camera >* comp = dynamic_cast< ModelComponent< Camera >* >( &component );
      if ( comp )
      {
         m_camera = &comp->get();
         return;
      }
   }

   {
      ModelComponent< ResourcesManager >* comp = dynamic_cast< ModelComponent< ResourcesManager >* >( &component );
      if ( comp )
      {
         // load the shader
         ResourcesManager& rm = comp->get();
         static const char* shaderName = "/Renderer/Shaders/SingleTextureEffect.fx";
         m_effect = dynamic_cast< EffectShader* >( rm.findResource( shaderName ) );
         if ( !m_effect )
         {
            m_effect = new EffectShader( shaderName );
            rm.addResource( m_effect );
         }
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
