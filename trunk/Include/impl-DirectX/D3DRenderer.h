#pragma once

#include "core-Renderer\Renderer.h"
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "core\Subject.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * D3DRenderer will emit these signals to its observers
 * when it needs them to release or recreate all the
 * resources that were created in the D3DPOOL_DEFAULT pool.
 */
enum D3DGraphResourceOp
{
   GRO_RELEASE_RES,
   GRO_CREATE_RES
};

///////////////////////////////////////////////////////////////////////////////

class D3DRenderer : public Renderer, 
                    public Subject<D3DRenderer, D3DGraphResourceOp>
{
private:
   IDirect3DDevice9* m_d3Device;
   D3DPRESENT_PARAMETERS m_creationParams;
   D3DCAPS9 m_caps;
   bool m_deviceLost;
   bool m_hardwareTLOn;
   D3DFORMAT m_optimalTextureFormat;
   D3DVIEWPORT9 m_viewport;

public:
   D3DRenderer(IDirect3DDevice9* d3Device, 
               D3DPRESENT_PARAMETERS creationParams,
               D3DCAPS9 caps,
               UINT frontBufferWidth,
               UINT frontBufferHeight,
               bool hardwareTLOn,
               D3DFORMAT optimalTextureFormat);
   ~D3DRenderer();

   IDirect3DDevice9& getD3Device() {return *m_d3Device;}
   bool isHardwareTLOn() const {return m_hardwareTLOn;}

   UINT getBackBufferWidth() const {return m_creationParams.BackBufferWidth;}
   UINT getBackBufferHeight() const {return m_creationParams.BackBufferHeight;}

   UINT getSimultaneouslyRenderedTexturesCount() const;
   UINT getMaxLightsCount() const;

   IDirect3DVertexBuffer9* createVertexBuffer(UINT length, DWORD usageFlags, DWORD fvf, D3DPOOL memoryPool);
   IDirect3DIndexBuffer9* createIndexBuffer(UINT length, DWORD usageFlags, D3DFORMAT format, D3DPOOL memoryPool);
   D3DFORMAT getOptimalTextureFormat() const {return m_optimalTextureFormat;}

protected:
   void resetViewport(unsigned int width, unsigned int height);
   void cleanAllTargets(unsigned int count);
   void renderingBegin();
   void renderingEnd();
   bool isGraphicsSystemReady() const;
   void attemptToRecoverGraphicsSystem();

private:
   void initRenderer();
};

///////////////////////////////////////////////////////////////////////////////
