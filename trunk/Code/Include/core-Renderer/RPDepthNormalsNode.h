/// @file   core-Renderer/RPDepthNormalsNode.h
/// @brief  a rendering pipeline node responsible for rendering scene's depth & normals buffers
#pragma once

#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/SceneRenderTreeBuilder.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;
class PixelShader;
class RPSceneBuilder;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node responsible for rendering scene's depth & normals buffers.
 */
class RPDepthNormalsNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPDepthNormalsNode )

private:
   RPMSceneId                          m_renderedSceneId;
   RPSceneBuilder*                     m_builder;

   TRuntimeVar< RenderTarget* >        m_renderTarget;

public:
   RPDepthNormalsNode();
   ~RPDepthNormalsNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onInitialize( RenderingPipelineMechanism& host ) const;
   void onDeinitialize( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
