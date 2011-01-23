/// @file   core-Renderer/RPHDRNode.h
/// @brief  a post-process effect that creates an HDR effect.
#pragma once

#include "core-Renderer/RPPostProcessNode.h"
#include "core/RuntimeData.h"


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
   float                            m_exposure;
   float                            m_gaussMultiplier;

   TRuntimeVar< Renderer* >         m_renderer;
   TRuntimeVar< PixelShader* >      m_hdrPass;
   TRuntimeVar< ShaderTexture* >    m_baseTex;
   TRuntimeVar< ShaderTexture* >    m_bloomedTex;
   TRuntimeVar< RenderTarget* >     m_hdrTarget;

public:
   RPHDRNode();
   ~RPHDRNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onInitialize( RenderingPipelineMechanism& host ) const;
   void onDeinitialize( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
