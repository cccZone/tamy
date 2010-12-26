#include "core-Renderer/RPSceneRenderNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/SpatialView.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/AttributeSorter.h"



///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSceneRenderNode, RenderingPipelineNode )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

void RPSceneRenderNode::update( RenderingPipelineMechanism& host )
{
   host.getStatesMgr().render();
}

///////////////////////////////////////////////////////////////////////////////

