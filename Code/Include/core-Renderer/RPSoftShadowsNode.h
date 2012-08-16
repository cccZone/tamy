/// @file   core-Renderer/RPSoftShadowsNode.h
/// @brief  a rendering pipeline node responsible for rendering scene shadows using a soft shadows generating technique
#pragma once

#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineNode.h"

#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;
class MemoryPool;
class PixelShader;
class DepthBuffer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node responsible for rendering scene shadows using a soft shadows generating technique.
 */
class RPSoftShadowsNode : public RenderingPipelineNode
{
   DECLARE_CLASS()

private:
   // static data
   std::string                               m_shadowDepthTextureId;
   std::string                               m_shadowDepthSurfaceId;
   std::string                               m_screenSpaceShadowMapId;

   // runtime data
   RPTextureOutput*                          m_shadowDepthTextureOutput;
   RPTextureOutput*                          m_screenSpaceShadowMapOutput;

   TRuntimeVar< RenderTarget* >              m_shadowDepthTexture;
   TRuntimeVar< DepthBuffer* >               m_shadowDepthSurface;
   TRuntimeVar< RenderTarget* >              m_screenSpaceShadowMap;
   TRuntimeVar< MemoryPool* >                m_treeMemPool;

public:
   /**
    * Constructor.
    */
   RPSoftShadowsNode();
   ~RPSoftShadowsNode();

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
