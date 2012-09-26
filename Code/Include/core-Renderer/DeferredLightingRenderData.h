/// @file   core-Renderer/DeferredLightingRenderData.h
/// @brief  deferred lighting renderer data
#pragma once

#include "core/Array.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingView;
class Geometry;
class RenderTarget;
class DepthBuffer;
class ShaderTexture;

///////////////////////////////////////////////////////////////////////////////

/**
 * A helper structure that aggregates all data needed to render shadows.
 */
struct DeferredLightingRenderData
{
   // data required by the shadow calculation shaders
   const RenderingView*             m_renderingView;
   const Array< Geometry* >*        m_geometryToRender;
   RenderTarget*                    m_shadowDepthTexture;
   DepthBuffer*                     m_shadowDepthSurface;
   RenderTarget*                    m_screenSpaceShadowMap;

   // data required by the lighting calculation shaders
   ShaderTexture*                   m_depthTex;
   ShaderTexture*                   m_normalsTex;
   ShaderTexture*                   m_specularTex;
   ShaderTexture*                   m_sceneColorTex;
   ShaderTexture*                   m_materialIndicesTex;
   ShaderTexture*                   m_materialsDescriptorsTex;
   RenderTarget*                    m_finalLightColorTarget;
};

///////////////////////////////////////////////////////////////////////////////
