/// @file   core-Renderer/RPBrightnessNode.h
/// @brief  a post-process effect that brightens an image.
#pragma once

#include "core-Renderer/RPPostProcessNode.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class ShaderTexture;
class Renderer;
class RenderTarget;

///////////////////////////////////////////////////////////////////////////////

/**
 * A post-process effect that brightens an image.
 */
class RPBrightnessNode : public RPPostProcessNode
{
   DECLARE_CLASS( RPBrightnessNode )

private:
   float                m_brightThreshold;

   Renderer*            m_renderer;
   PixelShader*         m_brightnessPass;
   ShaderTexture*       m_inputTex;
   RenderTarget*        m_brightPassTarget;

public:
   RPBrightnessNode();
   ~RPBrightnessNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onInitialize( RenderingPipelineMechanism& host );
   void onDeinitialize( RenderingPipelineMechanism& host );
   void onUpdate( RenderingPipelineMechanism& host );
};

///////////////////////////////////////////////////////////////////////////////
