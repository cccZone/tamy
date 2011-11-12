/// @file   core-Renderer/RPSceneRenderNode.h
/// @brief  a rendering pipeline node responsible for rendering a scene
#pragma once

#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/SceneRenderTreeBuilder.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node responsible for rendering a scene.
 */
class RPSceneRenderNode : public RenderingPipelineNode, public SceneRenderTreeBuilder
{
   DECLARE_CLASS( RPSceneRenderNode )

private:
   RPMSceneId                          m_renderedSceneId;

   TRuntimeVar< RenderTarget* >        m_renderTarget;

public:
   RPSceneRenderNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onInitialize( RenderingPipelineMechanism& host ) const;
   void onDeinitialize( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;

   // -------------------------------------------------------------------------
   // SceneRenderTreeBuilder implementation
   // -------------------------------------------------------------------------
   StateTreeNode* buildStateTree( const Array< SpatialRepresentation* >& visibleElems ) const;
};

///////////////////////////////////////////////////////////////////////////////
