/// @file   core-Renderer/DeferredAmbientLightRenderer.h
/// @brief  an ambient light renderer that uses a deferred rendering method
#pragma once


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class AmbientLight;
class ShaderTexture;
class RenderTarget;
class PixelShader;

///////////////////////////////////////////////////////////////////////////////

class DeferredAmbientLightRenderer
{
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
    * @param sceneColorTex       a texture with raw scene albedo
    * @param renderTarget        render target to which the lit scene should be drawn
    */
   void render( Renderer& renderer, const AmbientLight* light, ShaderTexture* sceneColorTex, RenderTarget* renderTarget );

};

///////////////////////////////////////////////////////////////////////////////
