#include "core-Renderer/RPRenderTargetNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/Renderer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPRenderTargetNode, RenderingPipelineNode )
   PROPERTY_EDIT( "Render target id", std::string, m_renderTargetId )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPRenderTargetNode::RPRenderTargetNode()
: m_renderTarget( NULL )
, m_renderer( NULL )
{

}

///////////////////////////////////////////////////////////////////////////////

RPRenderTargetNode::~RPRenderTargetNode()
{
   m_renderTarget = NULL;
   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPRenderTargetNode::initialize( RenderingPipelineMechanism& host )
{
   m_renderer = &host.getRenderer();
   m_renderTarget = &host.getRenderTarget( m_renderTargetId );
}

///////////////////////////////////////////////////////////////////////////////

void RPRenderTargetNode::deinitialize( RenderingPipelineMechanism& host )
{
   m_renderTarget = NULL;
   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPRenderTargetNode::update( RenderingPipelineMechanism& host )
{
   if ( m_renderer )
   {
      m_renderer->setRenderTarget( m_renderTarget );
   }
}

///////////////////////////////////////////////////////////////////////////////
