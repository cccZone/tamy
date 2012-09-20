/// @file   core-Renderer/AccumulatedLightingRenderData.h
/// @brief  data used for during index lights rendering
#pragma once

#include "core/Array.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingView;
class Geometry;
class RenderTarget;
class DepthBuffer;
class ShaderTexture;

///////////////////////////////////////////////////////////////////////////////

struct IndexedLightingRenderData
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
   RenderTarget*                    m_lightIndicesTarget;
};

///////////////////////////////////////////////////////////////////////////////