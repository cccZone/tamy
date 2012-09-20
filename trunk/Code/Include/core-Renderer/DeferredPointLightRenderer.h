/// @file   core-Renderer/DeferredPointLightRenderer.h
/// @brief  utility class that renders a point light using a deferred rendering method
#pragma once


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class PointLight;
struct DeferredLightingRenderData;
class VertexShader;
class PixelShader;
class TriangleMesh;

///////////////////////////////////////////////////////////////////////////////

/**
 * Utility class that renders a point light using a deferred rendering method.
 */
class DeferredPointLightRenderer
{
private:
   VertexShader*                             m_vertexShader;
   PixelShader*                              m_pixelShader;
   TriangleMesh*                             m_pointLightMesh;

public:
   /**
    * Constructor.
    */
   DeferredPointLightRenderer();
   ~DeferredPointLightRenderer();

   /**
    * Renders the light
    *
    * @param renderer
    * @param light
    * @param data
    */
   void render( Renderer& renderer, const PointLight* light, const DeferredLightingRenderData& data );
};

///////////////////////////////////////////////////////////////////////////////
