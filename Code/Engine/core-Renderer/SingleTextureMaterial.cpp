#include "core-Renderer\SingleTextureMaterial.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\RenderableTexture.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Defines.h"
#include "core-MVC.h"
#include "core.h"
#include <stdexcept>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( SingleTextureMaterial );
   PARENT( Entity );
   PROPERTY_EDIT( "material", SurfaceProperties, m_surfaceProperties );
   PROPERTY_EDIT( "texture", Texture*, m_texture );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

SingleTextureMaterial::SingleTextureMaterial( const std::string& name )
   : Entity( name )
   , m_shader( NULL )
   , m_texture( NULL )
   , m_renderableTexture( new RenderableTexture() )
{
   // load the shader
   static FilePath shaderName( SHADERS_DIR "PixelShaders/singleTextureMaterial.tpsh" );
   m_shader = ResourcesManager::getInstance().create< PixelShader >( shaderName );
   ASSERT_MSG( m_shader != NULL, "Shader used by the SingleTextureMaterial could not be loaded" )
}

///////////////////////////////////////////////////////////////////////////////

SingleTextureMaterial::~SingleTextureMaterial()
{
   delete m_renderableTexture;
   m_renderableTexture = NULL;

   m_shader = NULL;
   m_texture = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::onPreRender( Renderer& renderer ) const
{
   if ( !m_shader )
   {
      return;
   }
   
   RCBindPixelShader* comm = new ( renderer() ) RCBindPixelShader( *m_shader );

   comm->setVec4( "g_MaterialAmbientColor", ( const Vector& )m_surfaceProperties.getAmbientColor() );
   comm->setVec4( "g_MaterialDiffuseColor", ( const Vector& )m_surfaceProperties.getDiffuseColor() );
   comm->setBool( "g_UseTexture", m_texture != NULL );
   if ( m_texture != NULL )
   {
      m_renderableTexture->setTexture( m_texture );
      comm->setTexture( "g_MeshTexture", *m_renderableTexture );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureMaterial::onPostRender( Renderer& renderer ) const
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
