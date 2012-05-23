#include "core-Renderer\SingleTextureEffect.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\RenderableTexture.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\EffectShader.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\Renderer.h"
#include "core-MVC\SpatialEntity.h"
#include "core-Renderer\Defines.h"
#include "core-MVC.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( SingleTextureEffect );
   PARENT( Entity );
   PROPERTY_EDIT( "material", SurfaceProperties, m_surfaceProperties );
   PROPERTY_EDIT( "texture", Texture*, m_texture );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

SingleTextureEffect::SingleTextureEffect()
   : m_parentNode( NULL )
   , m_effect( NULL )
   , m_texture( NULL )
   , m_renderableTexture( new RenderableTexture() )
{
}

///////////////////////////////////////////////////////////////////////////////

SingleTextureEffect::~SingleTextureEffect()
{
   delete m_renderableTexture;
   m_renderableTexture = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::setSurfaceProperties( const SurfaceProperties& properties )
{
   m_surfaceProperties = properties;
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::setTexture( Texture& texture )
{
   m_texture = &texture;
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onPreRender( Renderer& renderer ) const
{
   if ( !m_parentNode || !m_effect )
   {
      return;
   }

   Camera& camera = renderer.getActiveCamera();
   RCBindEffect* comm = new ( renderer() ) RCBindEffect( *m_effect );

   Matrix worldViewMtx;
   worldViewMtx.setMul( m_parentNode->getGlobalMtx(), camera.getViewMtx() );
   comm->setMtx( "g_mWorldView", worldViewMtx );
   comm->setMtx( "g_mProjection", camera.getProjectionMtx() );

   comm->setVec4( "g_MaterialAmbientColor", ( const Vector& )m_surfaceProperties.getAmbientColor() );
   comm->setVec4( "g_MaterialDiffuseColor", ( const Vector& )m_surfaceProperties.getDiffuseColor() );

   comm->setBool( "g_UseTexture", m_texture != NULL );
   if ( m_texture != NULL )
   {
      m_renderableTexture->setTexture( m_texture );
      comm->setTexture( "g_MeshTexture", *m_renderableTexture );
   }

   comm->setTechnique( "singleTextureRenderer" );
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onPostRender( Renderer& renderer ) const
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
   return ( m_texture == rhs.m_texture ) && ( m_surfaceProperties == rhs.m_surfaceProperties );
}

///////////////////////////////////////////////////////////////////////////////

bool SingleTextureEffect::onLess( const SingleTextureEffect& rhs ) const
{
   return ( m_texture < rhs.m_texture ) && ( &m_surfaceProperties < &rhs.m_surfaceProperties );
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onAttached( Entity& parent )
{
   Geometry* geometry = dynamic_cast< Geometry* >( &parent );
   if ( geometry )
   {
      m_parentNode = geometry;
   }
}

///////////////////////////////////////////////////////////////////////////////

void SingleTextureEffect::onDetached( Entity& parent )
{
   Geometry* geometry = dynamic_cast< Geometry* >( &parent );
   if ( geometry )
   {
      m_parentNode = NULL;
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
      static FilePath shaderName( SHADERS_DIR "SingleTextureEffect.fx" );
      m_effect = rm.findResource< EffectShader >( shaderName );
      if ( !m_effect )
      {
         m_effect = new EffectShader( shaderName );
         
         // load the shader code
         File* shaderFile = rm.getFilesystem().open( shaderName, std::ios_base::in );
         StreamBuffer< char > shaderCodeBuf( *shaderFile );
         m_effect->setScript( shaderCodeBuf.getBuffer() );
         delete shaderFile;

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
