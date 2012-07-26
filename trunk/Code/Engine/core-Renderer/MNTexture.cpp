#include "core-Renderer/MNTexture.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/Texture.h"
#include "core-Renderer/RenderableTexture.h"
#include "core-Renderer/MaterialEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNTexture );
   PARENT( MaterialNode );
   PROPERTY_EDIT( "Texture", Texture*, m_texture );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MNTexture::MNTexture()
   : m_texture( NULL )
   , m_output( new MSTextureOutput( "Texture" ) )
   , m_renderableTexture( new RenderableTexture() )
{
   defineOutput( m_output );
}

///////////////////////////////////////////////////////////////////////////////

MNTexture::~MNTexture()
{
   delete m_renderableTexture;
   m_renderableTexture = NULL;

   m_texture = NULL;
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNTexture::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< MSTextureOutput >( findOutput( "Texture" ) );
}

///////////////////////////////////////////////////////////////////////////////

void MNTexture::preRender( Renderer& renderer, const MaterialEntity& entity ) const
{
   m_renderableTexture->setTexture( m_texture );
   m_output->setValue( entity.data(), m_renderableTexture );
}

///////////////////////////////////////////////////////////////////////////////