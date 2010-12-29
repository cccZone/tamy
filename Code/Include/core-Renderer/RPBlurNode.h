/// @file   core-Renderer/RPBlurNode.h
/// @brief  a post-process effect that blurs an image.
#pragma once

#include "core-Renderer/RPPostProcessNode.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class ShaderTexture;
class Renderer;
class RenderTarget;

///////////////////////////////////////////////////////////////////////////////

// TODO: !!!!!!!!!!!!! rejestorwanie enum'ow jako typow RTTI
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
   BlurOptions          m_blurOption;
   float                m_gaussMultiplier;
   float                m_gaussMean;
   float                m_gaussStdDev;

   Renderer*            m_renderer;
   PixelShader*         m_horizBlurPass;
   PixelShader*         m_vertBlurPass;
   ShaderTexture*       m_inputTex;
   RenderTarget*        m_blurTarget;
   RenderTarget*        m_tempBlurTarget;

public:
   RPBlurNode();
   ~RPBlurNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onInitialize( RenderingPipelineMechanism& host );
   void onDeinitialize( RenderingPipelineMechanism& host );
   void onUpdate( RenderingPipelineMechanism& host );

private:
   float computeGaussianValue( float x, float mean, float std_deviation ) const;
};

///////////////////////////////////////////////////////////////////////////////
