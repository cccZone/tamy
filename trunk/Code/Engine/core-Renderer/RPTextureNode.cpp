#include "core-Renderer/RPTextureNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/Texture.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPTextureNode, RenderingPipelineNode )
   PROPERTY_EDIT( "Texture", Texture*, m_texture )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPTextureNode::RPTextureNode()
   : m_texture( NULL )
   , m_output( new RPTextureOutput( "Texture" ) )
{
   defineOutput( m_output );
}

///////////////////////////////////////////////////////////////////////////////

RPTextureNode::~RPTextureNode()
{
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
   m_output->setValue( host.data(), m_texture );
}

///////////////////////////////////////////////////////////////////////////////
