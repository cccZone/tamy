#include "core-Renderer\SingleTextureMaterial.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Defines.h"
#include "core-MVC.h"
#include "core.h"
#include <stdexcept>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( SingleTextureMaterial, Entity )
   PROPERTY_EDIT( "material", SurfaceProperties, m_surfaceProperties )
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

void SingleTextureMaterial::onPreRender( Renderer& renderer, RuntimeDataBuffer& data ) const
{
   if ( !m_shader )
   {
      return;
   }
   
   RCBindPixelShader* comm = new ( renderer() ) RCBindPixelShader( *m_shader );

   comm->setVec4( "g_MaterialAmbientColor", ( D3DXVECTOR4 )m_surfaceProperties.getAmbientColor() );
   comm->setVec4( "g_MaterialDiffuseColor", ( D3DXVECTOR4 )m_surfaceProperties.getDiffuseColor() );
   comm->setBool( "g_UseTexture", m_texture != NULL );
   if ( m_texture != NULL )
   {
      comm->setTexture( "g_MeshTexture", *m_texture );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::onPostRender( Renderer& renderer, RuntimeDataBuffer& data ) const
{
   if ( !m_shader )
   {
      return;
   }

   new ( renderer() ) RCUnbindPixelShader( *m_shader );
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::onComponentAdded( Component< Model >& component )
{
   ModelComponent< ResourcesManager >* comp = dynamic_cast< ModelComponent< ResourcesManager >* >( &component );
   if ( comp )
   {
      // load the shader
      ResourcesManager& rm = comp->get();
      static FilePath shaderName( SHADERS_DIR "MaterialShader.psh" );
      m_shader = rm.findResource< PixelShader >( shaderName );
      if ( !m_shader )
      {
         m_shader = new PixelShader( shaderName );
         
         // load the shader code
         File* shaderFile = rm.getFilesystem().open( shaderName, std::ios_base::in );
         StreamBuffer< char > shaderCodeBuf( *shaderFile );
         m_shader->setScript( shaderCodeBuf.getBuffer() );
         delete shaderFile;

         rm.addResource( m_shader );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

Entity* SingleTextureMaterial::cloneSelf() const
{
   SingleTextureMaterial* entity = new SingleTextureMaterial( *this );
   return entity;
}

///////////////////////////////////////////////////////////////////////////////

bool SingleTextureMaterial::onEquals( const SingleTextureMaterial& rhs ) const
{
   return ( m_texture == rhs.m_texture ) && ( m_surfaceProperties == rhs.m_surfaceProperties );
}

///////////////////////////////////////////////////////////////////////////////

bool SingleTextureMaterial::onLess( const SingleTextureMaterial& rhs ) const
{
   return( m_texture < rhs.m_texture ) && ( &m_surfaceProperties < &rhs.m_surfaceProperties );
}

///////////////////////////////////////////////////////////////////////////////
