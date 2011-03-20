#include "core-Renderer/RPPostProcessNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/FullscreenQuad.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderTarget.h"


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
   host.data()[ m_fullscreenQuad ] = new FullscreenQuad();
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

void RPPostProcessNode::renderQuad( RuntimeDataBuffer& data, Renderer& renderer, RenderTarget* rt ) const
{
   // get the render target size
   unsigned int trgWidth, trgHeight;
   if ( rt )
   {
      trgWidth = rt->getWidth();
      trgHeight = rt->getHeight();
   }
   else
   {
      trgWidth = renderer.getViewportWidth();
      trgHeight = renderer.getViewportHeight();
   }

   renderer.setRenderTarget( rt );
   data[ m_fullscreenQuad ]->render( trgWidth, trgHeight );
}

///////////////////////////////////////////////////////////////////////////////
