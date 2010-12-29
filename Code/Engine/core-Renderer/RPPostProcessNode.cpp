#include "core-Renderer/RPPostProcessNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/FullscreenQuad.h"
#include "core-Renderer/Renderer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( RPPostProcessNode, RenderingPipelineNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPPostProcessNode::RPPostProcessNode()
   : m_fullscreenQuad( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

RPPostProcessNode::~RPPostProcessNode()
{
   delete m_fullscreenQuad;
   m_fullscreenQuad = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onInitialize( RenderingPipelineMechanism& host )
{
   Renderer& renderer = host.getRenderer();
   ResourcesManager& rm = ResourcesManager::getInstance();

   // fullscreen quad
   m_fullscreenQuad = new FullscreenQuad( &renderer );
   m_fullscreenQuad->initialize( rm );
   renderer.implement< FullscreenQuad >( *m_fullscreenQuad );
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onDeinitialize( RenderingPipelineMechanism& host )
{
   delete m_fullscreenQuad;
   m_fullscreenQuad = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::renderQuad()
{
   m_fullscreenQuad->render();
}

///////////////////////////////////////////////////////////////////////////////
