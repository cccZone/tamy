/// @file   core-Renderer/RPPostProcessNode.h
/// @brief  a rendering pipeline node responsible for applying a post process effect.
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node responsible for applying a post process effect.
 */
class RPPostProcessNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPPostProcessNode )

public:
   RPPostProcessNode();
   virtual ~RPPostProcessNode();

protected:
   /**
    * Renders the full screen quad geometry.
    *
    * @param renderer
    * @param rt         render target
    */
   void renderQuad( Renderer& renderer, RenderTarget* rt ) const;
};

///////////////////////////////////////////////////////////////////////////////
