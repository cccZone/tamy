/// @file   core-Renderer/RPPostProcessNode.h
/// @brief  a rendering pipeline node responsible for applying a post process effect.
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class FullscreenQuad;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node responsible for applying a post process effect.
 */
class RPPostProcessNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPPostProcessNode )

private:
   FullscreenQuad*                           m_fullscreenQuad;

public:
   RPPostProcessNode();
   virtual ~RPPostProcessNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onInitialize( RenderingPipelineMechanism& host );
   void onDeinitialize( RenderingPipelineMechanism& host );

protected:
   /**
    * Renders the full screen quad geometry.
    */
   void renderQuad();
};

///////////////////////////////////////////////////////////////////////////////
