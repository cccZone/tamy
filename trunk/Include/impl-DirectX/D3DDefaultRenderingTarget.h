#pragma once

#include "core-Renderer\RenderingTarget.h"
#include "core\Observer.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DRenderer;
enum D3DGraphResourceOp;

///////////////////////////////////////////////////////////////////////////////

/**
 * This rendering target will render directly to the back buffer.
 *
 * If added to the renderer as a valid target, it needs to
 * be attached as an observer to the rendering device as well
 * in order to comply with the device resets.
 */
class D3DDefaultRenderingTarget : public RenderingTarget,
                                  public Observer<D3DRenderer, D3DGraphResourceOp>
{
private:
   IDirect3DDevice9& m_d3Device;
   D3DRenderer& m_renderer;
   IDirect3DSurface9* m_backBuffer;

public:
   D3DDefaultRenderingTarget(IDirect3DDevice9& d3Device, D3DRenderer& renderer);
   ~D3DDefaultRenderingTarget();

   void use(unsigned char idx);

   void update(D3DRenderer& renderer);

   void update(D3DRenderer& renderer, const D3DGraphResourceOp& operation);
};

///////////////////////////////////////////////////////////////////////////////
