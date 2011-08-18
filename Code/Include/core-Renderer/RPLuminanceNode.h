/// @file   core-Renderer/RPLuminanceNode.h
/// @brief  a post-process effect that measures the image luminance.
#pragma once

#include "core-Renderer/RPPostProcessNode.h"
#include "core/RuntimeData.h"
#include "core/Color.h"


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
   typedef RenderTarget*                           PRenderTarget;
private:
   unsigned int                                    m_stepsCount;

   TRuntimeVar< Renderer* >                        m_renderer;
   TRuntimeVar< PixelShader* >                     m_greyscalePass;
   TRuntimeVar< PixelShader* >                     m_downSamplePass;
   TRuntimeVar< ShaderTexture* >                   m_inputTex;
   TRuntimeVar< PRenderTarget* >                   m_luminanceTarget;
   TRuntimeVar< Color >                            m_luminanceVal;

public:
   RPLuminanceNode();
   ~RPLuminanceNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onInitialize( RenderingPipelineMechanism& host ) const;
   void onDeinitialize( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
