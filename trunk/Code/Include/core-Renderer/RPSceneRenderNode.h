/// @file   core-Renderer/RPSceneRenderNode.h
/// @brief  a rendering pipeline node responsible for rendering a scene
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node responsible for rendering a scene.
 */
class RPSceneRenderNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPSceneRenderNode )

private:
   RenderTarget*        m_renderTarget;

public:
   RPSceneRenderNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onInitialize( RenderingPipelineMechanism& host );
   void onDeinitialize( RenderingPipelineMechanism& host );
   void onUpdate( RenderingPipelineMechanism& host );
};

///////////////////////////////////////////////////////////////////////////////
