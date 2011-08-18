#include "core-Renderer\SingleTextureMaterial.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\Renderer.h"
#include "core-MVC.h"
#include "core.h"
#include <stdexcept>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( SingleTextureMaterial, Entity )
   PROPERTY_EDIT( "material", Material, m_material )
   PROPERTY_EDIT( "texture", Texture*, m_texture )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

SingleTextureMaterial::SingleTextureMaterial( const std::string& name )
   : Entity( name )
   , m_shader( NULL )
   , m_texture( NULL )
{}

///////////////////////////////////////////////////////////////////////////////

SingleTextureMaterial::~SingleTextureMaterial()
{
   m_shader = NULL;
   m_texture = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::onPreRender( Renderer& renderer )
{
   if ( !m_shader )
   {
      return;
   }
   
   RCBindPixelShader* comm = new ( renderer() ) RCBindPixelShader( *m_shader );

   comm->setVec4( "g_MaterialAmbientColor", ( D3DXVECTOR4 )m_material.getAmbientColor() );
   comm->setVec4( "g_MaterialDiffuseColor", ( D3DXVECTOR4 )m_material.getDiffuseColor() );
   comm->setBool( "g_UseTexture", m_texture != NULL );
   if ( m_texture != NULL )
   {
      comm->setTexture( "g_MeshTexture", *m_texture );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::onPostRender( Renderer& renderer )
{
   if ( !m_shader )
   {
      return;
   }

   new ( renderer() ) RCUnbindPixelShader( *m_shader );
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::onAttached( Entity& parent )
{
   Geometry* geometry = dynamic_cast< Geometry* >( &parent );
   if ( geometry )
   {
      geometry->addState( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::onDetached( Entity& parent )
{
   Geometry* geometry = dynamic_cast< Geometry* >( &parent );
   if ( geometry )
   {
      geometry->removeState( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::onComponentAdded( Component< Model >& component )
{
   ModelComponent< ResourcesManager >* comp = dynamic_cast< ModelComponent< ResourcesManager >* >( &component );
   if ( comp )
   {
      // load the shader
      ResourcesManager& rm = comp->get();
      static const char* shaderName = "Renderer/Shaders/MaterialShader.psh";
      m_shader = dynamic_cast< PixelShader* >( rm.findResource( "SingleTextureMaterial" ) );
      if ( !m_shader )
      {
         m_shader = new PixelShader( "SingleTextureMaterial" );
         m_shader->loadFromFile( rm.getFilesystem(), shaderName );
         rm.addResource( m_shader );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
