/// @file   core-Renderer/RPAccumulatedLightingNode.h
/// @brief  this node renders indices of lights affecting the visible scene
#pragma once

#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RPTextureInput;
class RPTextureOutput;
class RenderTarget;
class DepthBuffer;
class IndexedDirectionalLightRenderer;

// TODO: !!!!!!!!! implement me, right after you have the materials DB up and running
// use this white-paper: /Docs/LightIndexedDeferredLighting1.1.pdf

///////////////////////////////////////////////////////////////////////////////

/**
 * This node renders indices of lights affecting the visible scene.
 *
 * It's much like the deferred lighting node, with an exception that it doesn't apply the scene color or any
 * material data to the output image right away.
 *
 * Instead, if a light lights a visible surface, its index is stored ( the index is valid only during current
 * rendering frame )  so that future pipeline stages can take it, find a corresponding light
 *  and combine it with the scene colors and materials etc. in a way they see fit.
 */
class RPLightIndicesNode : public RenderingPipelineNode
{
   DECLARE_CLASS()

private:
   std::string                                           m_lightsColorTargetId;
   std::string                                           m_lightsDirectionTargetId;
   std::string                                           m_shadowDepthBufferId;
   std::string                                           m_shadowDepthSurfaceId;
   std::string                                           m_screenSpaceShadowMapId;

   // runtime data
   TRuntimeVar< RenderTarget* >                          m_lightsColorTarget;
   TRuntimeVar< RenderTarget* >                          m_lightsDirectionTarget;
   TRuntimeVar< RenderTarget* >                          m_shadowDepthBuffer;
   TRuntimeVar< DepthBuffer* >                           m_shadowDepthSurface;
   TRuntimeVar< RenderTarget* >                          m_screenSpaceShadowMap;

   TRuntimeVar< IndexedDirectionalLightRenderer* >       m_directionalLightsRenderer;


   // sockets data
   RPTextureInput*                                       m_normalsInput;
   RPTextureInput*                                       m_depthInput;

   RPTextureOutput*                                      m_lightsColorOutput;
   RPTextureOutput*                                      m_lightsDirectionOutput;
   RPTextureOutput*                                      m_shadowDepthBufferOutput;
   RPTextureOutput*                                      m_screenSpaceShadowMapOutput;


public:
   /**
    * Constructor.
    */
   RPLightIndicesNode();
   ~RPLightIndicesNode();

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
