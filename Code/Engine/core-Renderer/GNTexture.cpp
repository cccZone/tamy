#include "core-Renderer/GNTexture.h"
#include "core-Renderer/GeometrySockets.h"
#include "core-Renderer/Texture.h"
#include "core-Renderer/RenderableTexture.h"
#include "core-Renderer/GeometryEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GNTexture );
   PARENT( GeometryShaderNode );
   PROPERTY_EDIT( "Texture", Texture*, m_texture );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GNTexture::GNTexture()
   : m_texture( NULL )
   , m_output( new GSTextureOutput( "Texture" ) )
   , m_renderableTexture( new RenderableTexture() )
{
   bool isBeingDeserialized = SerializationFlag::getInstance().isSerializationInProgress();
   if ( !isBeingDeserialized )
   {
      m_output = new GSTextureOutput( "Texture" );
      defineOutput( m_output );
   }
}

///////////////////////////////////////////////////////////////////////////////

GNTexture::~GNTexture()
{
   delete m_renderableTexture;
   m_renderableTexture = NULL;

   m_texture = NULL;
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GNTexture::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< GSTextureOutput >( findOutput( "Texture" ) );
}

///////////////////////////////////////////////////////////////////////////////

void GNTexture::preRender( Renderer& renderer, const GeometryEntity& entity ) const
{
   m_renderableTexture->setTexture( m_texture );
   m_output->setValue( entity.data(), m_renderableTexture );
}

///////////////////////////////////////////////////////////////////////////////
