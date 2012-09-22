#include "core-Renderer/MNInstanceTexture.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/Texture.h"
#include "core-Renderer/RenderableTexture.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNInstanceTexture );
   PARENT( MaterialNode );
   PROPERTY_EDIT( "Texture usage", MaterialTextures, m_usage );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MNInstanceTexture::MNInstanceTexture()
   : m_usage( MT_DIFFUSE_1 )
   , m_textureOutput( new MSTextureOutput( "Texture" ) )
   , m_isTextureSetOutput( new MSBoolOutput( "IsSet" ) )
   , m_renderableTexture( new RenderableTexture() )
{
   std::vector< GBNodeOutput< MaterialNode >* > outputs;
   outputs.push_back( m_textureOutput );
   outputs.push_back( m_isTextureSetOutput );

   defineOutputs( outputs );
}

///////////////////////////////////////////////////////////////////////////////

MNInstanceTexture::~MNInstanceTexture()
{
   delete m_renderableTexture;
   m_renderableTexture = NULL;

   m_textureOutput = NULL;
   m_isTextureSetOutput = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNInstanceTexture::onObjectLoaded()
{
   __super::onObjectLoaded();

   // delete the old output instances and replace them with the loaded ones
   delete m_textureOutput;
   delete m_isTextureSetOutput;

   m_textureOutput = DynamicCast< MSTextureOutput >( findOutput( "Texture" ) );
   m_isTextureSetOutput = DynamicCast< MSBoolOutput >( findOutput( "IsSet" ) );
}

///////////////////////////////////////////////////////////////////////////////

void MNInstanceTexture::preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const
{
   Texture* tex = instance.getTexture( m_usage );

   m_renderableTexture->setTexture( tex );
   m_textureOutput->setValue( instance.data(), m_renderableTexture );

   if ( m_isTextureSetOutput )
   {
      m_isTextureSetOutput->setValue( instance.data(), tex != NULL );
   }
}

///////////////////////////////////////////////////////////////////////////////
