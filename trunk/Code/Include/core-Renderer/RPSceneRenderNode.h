/// @file   core-Renderer/RPSceneRenderNode.h
/// @brief  a rendering pipeline node responsible for rendering a scene
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RPSceneRenderNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPSceneRenderNode )

public:
   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void update( RenderingPipelineMechanism& host );
};

///////////////////////////////////////////////////////////////////////////////
