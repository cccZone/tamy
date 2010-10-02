#include "core-Renderer\SingleTextureMaterial.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Texture.h"
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
   , m_camera( NULL )
   , m_geometry( NULL )
   , m_texture( NULL )
{}

///////////////////////////////////////////////////////////////////////////////

SingleTextureMaterial::~SingleTextureMaterial()
{
   m_attributes.clear();
   m_shader = NULL;
   m_camera = NULL;
   m_geometry = NULL;
   m_texture = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::render()
{
   if ( !m_camera || !m_shader || !m_geometry )
   {
      return;
   }
   
   m_shader->setVec4( "g_MaterialAmbientColor", ( D3DXVECTOR4 )m_material.getAmbientColor() );
   m_shader->setVec4( "g_MaterialDiffuseColor", ( D3DXVECTOR4 )m_material.getDiffuseColor() );
   m_shader->setBool( "g_UseTexture", m_texture != NULL );
   if ( m_texture != NULL )
   {
      m_shader->setTexture( "g_MeshTexture", *m_texture );
   }
   m_shader->beginRendering();
   m_geometry->render();
   m_shader->endRendering();
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::onAttached( Entity& parent )
{
   m_geometry = dynamic_cast< Geometry* >( &parent );
   m_attributes.push_back( new EffectAttribute() );
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::onDetached( Entity& parent )
{
   m_geometry = NULL;

   delete m_attributes[0];
   m_attributes.clear();
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::onComponentAdded( Component< Model >& component )
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
         static const char* shaderName = "Renderer/Shaders/MaterialShader.psh";
         m_shader = dynamic_cast< PixelShader* >( rm.findResource( "SingleTextureMaterial" ) );
         if ( !m_shader )
         {
            m_shader = new PixelShader( "SingleTextureMaterial" );
            m_shader->loadFromFile( rm.getFilesystem(), shaderName );
            rm.addResource( m_shader );
         }

         return;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
