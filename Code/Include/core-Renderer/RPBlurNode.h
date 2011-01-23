/// @file   core-Renderer/RPBlurNode.h
/// @brief  a post-process effect that blurs an image.
#pragma once

#include "core-Renderer/RPPostProcessNode.h"
#include "core/Enum.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class ShaderTexture;
class Renderer;
class RenderTarget;

///////////////////////////////////////////////////////////////////////////////

enum BlurOptions
{
   BO_HORIZONTAL,
   BO_VERTICAL,
   BO_BOTH
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A post-process effect that blurs an image.
 */
class RPBlurNode : public RPPostProcessNode
{
   DECLARE_CLASS( RPBlurNode )

private:
   BlurOptions                      m_blurOption;
   float                            m_gaussMultiplier;
   float                            m_gaussMean;
   float                            m_gaussStdDev;

   TRuntimeVar< Renderer* >         m_renderer;
   TRuntimeVar< PixelShader* >      m_horizBlurPass;
   TRuntimeVar< PixelShader* >      m_vertBlurPass;
   TRuntimeVar< ShaderTexture* >    m_inputTex;
   TRuntimeVar< RenderTarget* >     m_blurTarget;
   TRuntimeVar< RenderTarget* >     m_tempBlurTarget;

public:
   RPBlurNode();
   ~RPBlurNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onInitialize( RenderingPipelineMechanism& host ) const;
   void onDeinitialize( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;

private:
   float computeGaussianValue( float x, float mean, float std_deviation ) const;
};

///////////////////////////////////////////////////////////////////////////////
