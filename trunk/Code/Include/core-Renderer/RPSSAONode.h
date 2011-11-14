/// @file   core-Renderer/RPSSAONode.h
/// @brief  a post-process effect that calculates screen space ambient occlusion.
#pragma once

#include "core-Renderer/RPPostProcessNode.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class ShaderTexture;
class Renderer;
class RenderTarget;

///////////////////////////////////////////////////////////////////////////////

/**
 * A post-process effect that calculates screen space ambient occlusion.
 */
class RPSSAONode : public RPPostProcessNode
{
   DECLARE_CLASS( RPSSAONode )

private:
   // static data
   float                            m_occlusionRadius;
   float                            m_noOcclusionThreshold;
   float                            m_fullOcclusionThreshold;
   float                            m_occlusionPower;

   // runtime data
   TRuntimeVar< Renderer* >         m_renderer;
   TRuntimeVar< PixelShader* >      m_ssaoPass;
   TRuntimeVar< ShaderTexture* >    m_normalsAndDepthBuffer;
   TRuntimeVar< RenderTarget* >     m_outputTarget;

public:
   RPSSAONode();
   ~RPSSAONode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onInitialize( RenderingPipelineMechanism& host ) const;
   void onDeinitialize( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
