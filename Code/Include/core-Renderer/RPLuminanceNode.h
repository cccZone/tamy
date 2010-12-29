/// @file   core-Renderer/RPLuminanceNode.h
/// @brief  a post-process effect that measures the image luminance.
#pragma once

#include "core-Renderer/RPPostProcessNode.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class ShaderTexture;
class Renderer;
class RenderTarget;
class RPFloatOutput;

///////////////////////////////////////////////////////////////////////////////

/**
 * A post-process effect that brightens an image.
 */
class RPLuminanceNode : public RPPostProcessNode
{
   DECLARE_CLASS( RPLuminanceNode )

private:
   unsigned int                        m_stepsCount;

   Renderer*                           m_renderer;
   PixelShader*                        m_greyscalePass;
   PixelShader*                        m_downSamplePass;
   ShaderTexture*                      m_inputTex;
   RPFloatOutput*                       m_maxLuminanceOutput;
   RPFloatOutput*                       m_avgLuminanceOutput;

   std::vector< RenderTarget* >        m_luminanceTarget;

public:
   RPLuminanceNode();
   ~RPLuminanceNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onInitialize( RenderingPipelineMechanism& host );
   void onDeinitialize( RenderingPipelineMechanism& host );
   void onUpdate( RenderingPipelineMechanism& host );
};

///////////////////////////////////////////////////////////////////////////////
