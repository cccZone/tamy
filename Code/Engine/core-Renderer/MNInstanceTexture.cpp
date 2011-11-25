#include "core-Renderer/MNInstanceTexture.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/Texture.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNInstanceTexture, MaterialNode )
   PROPERTY_EDIT( "Texture usage", MaterialTextures, m_usage )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

MNInstanceTexture::MNInstanceTexture()
   : m_usage( MT_DIFFUSE_1 )
   , m_output( new MSTextureOutput( "Texture" ) )
{
   defineOutput( m_output );
}

///////////////////////////////////////////////////////////////////////////////

MNInstanceTexture::~MNInstanceTexture()
{
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNInstanceTexture::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< MSTextureOutput >( findOutput( "Texture" ) );
}

///////////////////////////////////////////////////////////////////////////////

void MNInstanceTexture::preRender( Renderer& renderer, const MaterialEntity& entity ) const
{
   m_output->setValue( entity.data(), entity.getTexture( m_usage ) );
}

///////////////////////////////////////////////////////////////////////////////
