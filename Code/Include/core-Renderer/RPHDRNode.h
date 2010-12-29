/// @file   core-Renderer/RPHDRNode.h
/// @brief  a post-process effect that creates an HDR effect.
#pragma once

#include "core-Renderer/RPPostProcessNode.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class ShaderTexture;
class Renderer;
class RenderTarget;
class RPFloatInput;

///////////////////////////////////////////////////////////////////////////////

/**
 * A post-process effect that creates an HDR effect.
 */
class RPHDRNode : public RPPostProcessNode
{
   DECLARE_CLASS( RPHDRNode )

private:
   float                m_exposure;
   float                m_gaussMultiplier;

   Renderer*            m_renderer;
   PixelShader*         m_hdrPass;
   ShaderTexture*       m_baseTex;
   ShaderTexture*       m_bloomedTex;
   RPFloatInput*        m_maxLuminanceInput;
   RPFloatInput*        m_avgLuminanceInput;
   RenderTarget*        m_hdrTarget;

public:
   RPHDRNode();
   ~RPHDRNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onInitialize( RenderingPipelineMechanism& host );
   void onDeinitialize( RenderingPipelineMechanism& host );
   void onUpdate( RenderingPipelineMechanism& host );
};

///////////////////////////////////////////////////////////////////////////////
