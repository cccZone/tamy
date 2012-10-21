/// @file   core-Renderer/DeferredAmbientLightRenderer.h
/// @brief  an ambient light renderer that uses a deferred rendering method
#pragma once

#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class AmbientLight;
class ShaderTexture;
class RenderTarget;
class PixelShader;
struct DeferredLightingRenderData;

///////////////////////////////////////////////////////////////////////////////

class DeferredAmbientLightRenderer
{
   DECLARE_ALLOCATOR( DeferredAmbientLightRenderer, AM_DEFAULT );

private:
   PixelShader*      m_shader;

public:
   /**
    * Constructor.
    */
   DeferredAmbientLightRenderer();
   ~DeferredAmbientLightRenderer();

   /**
    * Renders the ambient light.
    *
    * @param renderer
    * @param light
    * @param data         light rendering data
    */
   void render( Renderer& renderer, const AmbientLight* light, const DeferredLightingRenderData& data );

};

///////////////////////////////////////////////////////////////////////////////
