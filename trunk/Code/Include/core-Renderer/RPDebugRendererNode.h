/// @file   core-Renderer/RPDebugRendererNode.h
/// @brief  a rendering pipeline node that renders the debug info on the screen.
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"
#include "core/IDebugDraw.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node that renders the debug info on the screen.
 */
class RPDebugRendererNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPDebugRendererNode )

private:
   TRuntimeVar< RenderTarget* >                    m_renderTarget;

public:
   RPDebugRendererNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onInitialize( RenderingPipelineMechanism& host ) const;
   void onDeinitialize( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
