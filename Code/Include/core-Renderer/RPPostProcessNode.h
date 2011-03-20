/// @file   core-Renderer/RPPostProcessNode.h
/// @brief  a rendering pipeline node responsible for applying a post process effect.
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class FullscreenQuad;
class RenderTarget;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node responsible for applying a post process effect.
 */
class RPPostProcessNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPPostProcessNode )

private:
   TRuntimeVar< FullscreenQuad* >               m_fullscreenQuad;

public:
   RPPostProcessNode();
   virtual ~RPPostProcessNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onInitialize( RenderingPipelineMechanism& host ) const;
   void onDeinitialize( RenderingPipelineMechanism& host ) const;

protected:
   /**
    * Renders the full screen quad geometry.
    *
    * @param data       runtime data buffer
    * @param renderer
    * @param rt         render target
    */
   void renderQuad( RuntimeDataBuffer& data, Renderer& renderer, RenderTarget* rt ) const;
};

///////////////////////////////////////////////////////////////////////////////
