#include "core-Renderer/RPPostProcessNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/FullscreenQuad.h"
#include "core-Renderer/Renderer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( RPPostProcessNode, RenderingPipelineNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPPostProcessNode::RPPostProcessNode()
{
}

///////////////////////////////////////////////////////////////////////////////

RPPostProcessNode::~RPPostProcessNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   host.data().registerVar( m_fullscreenQuad );
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onInitialize( RenderingPipelineMechanism& host ) const
{
   Renderer& renderer = host.getRenderer();
   ResourcesManager& rm = ResourcesManager::getInstance();

   // fullscreen quad
   host.data()[ m_fullscreenQuad ] = new FullscreenQuad( &renderer );
   host.data()[ m_fullscreenQuad ]->initialize( rm );
   renderer.implement< FullscreenQuad >( *host.data()[ m_fullscreenQuad ] );
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onDeinitialize( RenderingPipelineMechanism& host ) const
{
   delete host.data()[ m_fullscreenQuad ];
   host.data()[ m_fullscreenQuad ] = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::renderQuad( RuntimeDataBuffer& data ) const
{
   data[ m_fullscreenQuad ]->render();
}

///////////////////////////////////////////////////////////////////////////////
