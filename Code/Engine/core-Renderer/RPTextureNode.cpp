#include "core-Renderer/RPTextureNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/Texture.h"
#include "core-Renderer/RenderableTexture.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPTextureNode );
   PARENT( RenderingPipelineNode );
   PROPERTY_EDIT( "Texture", Texture*, m_texture );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPTextureNode::RPTextureNode()
   : m_texture( NULL )
   , m_output( new RPTextureOutput( "Texture" ) )
   , m_renderableTexture( new RenderableTexture() )
{
   defineOutput( m_output );
}

///////////////////////////////////////////////////////////////////////////////

RPTextureNode::~RPTextureNode()
{
   delete m_renderableTexture;
   m_renderableTexture = NULL;

   m_texture = NULL;
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPTextureNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< RPTextureOutput >( findOutput( "Texture" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPTextureNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   m_renderableTexture->setTexture( m_texture );
   m_output->setValue( host.data(), m_renderableTexture );
}

///////////////////////////////////////////////////////////////////////////////
