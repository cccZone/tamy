/// @file   core-Renderer/RPDownsampleNode.h
/// @brief  a post-process effect that down samples an image.
#pragma once

#include "core-Renderer/RPPostProcessNode.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class ShaderTexture;
class Renderer;
class RenderTarget;

///////////////////////////////////////////////////////////////////////////////

/**
 * A post-process effect that down samples an image.
 */
class RPDownsampleNode : public RPPostProcessNode
{
   DECLARE_CLASS( RPDownsampleNode )

private:
   Renderer*            m_renderer;
   PixelShader*         m_downsamplePass;
   ShaderTexture*       m_inputTex;
   RenderTarget*        m_downsampleTarget;

public:
   RPDownsampleNode();
   ~RPDownsampleNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onInitialize( RenderingPipelineMechanism& host );
   void onDeinitialize( RenderingPipelineMechanism& host );
   void onUpdate( RenderingPipelineMechanism& host );
};

///////////////////////////////////////////////////////////////////////////////
