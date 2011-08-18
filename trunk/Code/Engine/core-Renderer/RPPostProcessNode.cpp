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

void RPPostProcessNode::renderQuad( Renderer& renderer, RenderTarget* rt ) const
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

   new ( renderer() ) RCActivateRenderTarget( rt );
   new ( renderer() ) RCFullscreenQuad( trgWidth, trgHeight );
}

///////////////////////////////////////////////////////////////////////////////
