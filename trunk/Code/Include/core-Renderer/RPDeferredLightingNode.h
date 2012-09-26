/// @file   core-Renderer/RPDeferredLightingNode.h
/// @brief  a node that provides information about the scene lights
#pragma once

#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;
class RPTextureInput;
class RPTextureOutput;
class PixelShader;
class VertexShader;
class TriangleMesh;
class DeferredDirectionalLightRenderer;
class DeferredPointLightRenderer;
class DeferredAmbientLightRenderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A node that provides information about the scene lights visible from the active camera.
 */
class RPDeferredLightingNode : public RenderingPipelineNode
{
   DECLARE_CLASS()

private:
   // runtime data
   TRuntimeVar< RenderTarget* >                          m_shadowDepthTexture;
   TRuntimeVar< DepthBuffer* >                           m_shadowDepthSurface;
   TRuntimeVar< RenderTarget* >                          m_screenSpaceShadowMap;
   TRuntimeVar< RenderTarget* >                          m_finalLightColorTarget;

   TRuntimeVar< DeferredDirectionalLightRenderer* >      m_directionalLightsRenderer;
   TRuntimeVar< DeferredPointLightRenderer* >            m_pointLightsRenderer;
   TRuntimeVar< DeferredAmbientLightRenderer* >          m_ambientLightRenderer;

   // sockets data
   RPTextureInput*                                       m_sceneColorInput;
   RPTextureInput*                                       m_normalsInput;
   RPTextureInput*                                       m_specularInput;
   RPTextureInput*                                       m_depthInput;
   RPTextureInput*                                       m_materialIndicesInput;
   RPTextureInput*                                       m_materialsDescInput;

   RPTextureOutput*                                      m_finalLightColorTargetOutput;
   RPTextureOutput*                                      m_shadowDepthTextureOutput;
   RPTextureOutput*                                      m_screenSpaceShadowMapOutput;


public:
   /**
    * Constructor.
    */
   RPDeferredLightingNode();
   ~RPDeferredLightingNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreatePrerequisites( RenderingPipelineTransaction& transaction ) const;
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onDestroyLayout( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
};

///////////////////////////////////////////////////////////////////////////////
