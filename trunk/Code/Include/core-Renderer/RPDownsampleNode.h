/// @file   core-Renderer/RPDownsampleNode.h
/// @brief  a post-process effect that down samples an image.
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
 * A post-process effect that down samples an image.
 */
class RPDownsampleNode : public RPPostProcessNode
{
   DECLARE_CLASS( RPDownsampleNode )

private:
   TRuntimeVar< Renderer* >         m_renderer;
   TRuntimeVar< PixelShader* >      m_downsamplePass;
   TRuntimeVar< ShaderTexture* >    m_inputTex;
   TRuntimeVar< RenderTarget* >     m_downsampleTarget;

public:
   RPDownsampleNode();
   ~RPDownsampleNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onInitialize( RenderingPipelineMechanism& host ) const;
   void onDeinitialize( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
