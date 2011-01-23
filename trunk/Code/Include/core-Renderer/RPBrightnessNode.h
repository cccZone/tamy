/// @file   core-Renderer/RPBrightnessNode.h
/// @brief  a post-process effect that brightens an image.
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
 * A post-process effect that brightens an image.
 */
class RPBrightnessNode : public RPPostProcessNode
{
   DECLARE_CLASS( RPBrightnessNode )

private:
   float                            m_brightThreshold;

   TRuntimeVar< Renderer* >         m_renderer;
   TRuntimeVar< PixelShader* >      m_brightnessPass;
   TRuntimeVar< ShaderTexture* >    m_inputTex;
   TRuntimeVar< RenderTarget* >     m_brightPassTarget;

public:
   RPBrightnessNode();
   ~RPBrightnessNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onInitialize( RenderingPipelineMechanism& host ) const;
   void onDeinitialize( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
