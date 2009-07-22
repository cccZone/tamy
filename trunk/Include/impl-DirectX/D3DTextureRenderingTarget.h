#pragma once

#include "core-Renderer\TextureRenderingTarget.h"
#include "core\Observer.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DRenderer;
enum D3DGraphResourceOp;

///////////////////////////////////////////////////////////////////////////////

/**
 * This rendering target will render to a texture it also is
 * and can be used as.
 *
 * If added to the renderer as a valid target, it needs to
 * be attached as an observer to the rendering device as well
 * in order to comply with the device resets.
 */
class D3DTextureRenderingTarget : public TextureRenderingTarget,
                                  public Observer<D3DRenderer, D3DGraphResourceOp>
{
private:
   IDirect3DDevice9& m_d3Device;
   D3DRenderer& m_renderer;
   TTextureImpl<IDirect3DTexture9>* m_texImpl;

   IDirect3DTexture9* m_texture;
   IDirect3DSurface9* m_surface;

public:
   D3DTextureRenderingTarget(const std::string& name,
                             IDirect3DDevice9& d3Device, 
                             D3DRenderer& renderer);
   ~D3DTextureRenderingTarget();

   TextureImpl& getImpl();

   void use(unsigned char idx);

   void update(D3DRenderer& renderer);

   void update(D3DRenderer& renderer, const D3DGraphResourceOp& operation);

private:
   void releaseTexture();
   void createTexture(D3DRenderer& renderer);
};

///////////////////////////////////////////////////////////////////////////////
