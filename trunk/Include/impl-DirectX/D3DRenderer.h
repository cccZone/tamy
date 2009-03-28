#pragma once

#include "core-Renderer\Renderer.h"
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DRenderer : public Renderer
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

protected:
   void initRenderer();
   void resetViewport(unsigned int width, unsigned int height);

   UINT getMaxLightsCount() const;

   void setViewMatrix(const D3DXMATRIX& mtx);
   void setProjectionMatrix(const D3DXMATRIX& mtx);
   void renderingBegin();
   void renderingEnd();
   bool isGraphicsSystemReady() const;
   void attemptToRecoverGraphicsSystem();
};

///////////////////////////////////////////////////////////////////////////////
