/// @file   core-Renderer/RPDeferredLightingNode.h
/// @brief  a node that provides information about the scene lights
#pragma once

#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;
class RPTextureInput;
class RPTextureOutput;

///////////////////////////////////////////////////////////////////////////////

/**
 * A node that provides information about the scene lights visible from the active camera.
 */
class RPDeferredLightingNode : public RenderingPipelineNode
{
   DECLARE_CLASS()

private:
   std::string                               m_finalLightColorTargetId;
   std::string                               m_shadowDepthTextureId;
   std::string                               m_shadowDepthSurfaceId;
   std::string                               m_screenSpaceShadowMapId;

   // runtime data
   TRuntimeVar< RenderTarget* >              m_shadowDepthTexture;
   TRuntimeVar< DepthBuffer* >               m_shadowDepthSurface;
   TRuntimeVar< RenderTarget* >              m_screenSpaceShadowMap;
   TRuntimeVar< RenderTarget* >              m_finalLightColorTarget;

   // sockets data
   RPTextureInput*                           m_sceneColorInput;
   RPTextureInput*                           m_normalsInput;
   RPTextureInput*                           m_specularInput;
   RPTextureInput*                           m_depthInput;

   RPTextureOutput*                          m_finalLightColorTargetOutput;
   RPTextureOutput*                          m_shadowDepthTextureOutput;
   RPTextureOutput*                          m_screenSpaceShadowMapOutput;


public:
   /**
    * Constructor.
    */
   RPDeferredLightingNode();
   ~RPDeferredLightingNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onDestroyLayout( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
   void onPropertyChanged( ReflectionProperty& property );
};

///////////////////////////////////////////////////////////////////////////////
