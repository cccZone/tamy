#include "core-Renderer/MNTexture.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/Texture.h"
#include "core-Renderer/MaterialEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNTexture, MaterialNode )
   PROPERTY_EDIT( "Texture", Texture*, m_texture )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

MNTexture::MNTexture()
   : m_texture( NULL )
   , m_output( new MSTextureOutput( "Texture" ) )
{
   defineOutput( m_output );
}

///////////////////////////////////////////////////////////////////////////////

MNTexture::~MNTexture()
{
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
   m_output->setValue( entity.data(), m_texture );
}

///////////////////////////////////////////////////////////////////////////////
