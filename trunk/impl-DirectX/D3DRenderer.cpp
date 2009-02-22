#include "D3DRenderer.h"
#include <stdexcept>
#include <string>
#include <cassert>
#include "GraphicalNode.h"
#include "D3DLight.h"
#include "RenderingCommand.h"


/////////////////////////////////////////////////////////////////////////////

D3DRenderer::D3DRenderer(IDirect3DDevice9* d3Device, 
                         D3DPRESENT_PARAMETERS creationParams,
                         D3DCAPS9 caps,
                         UINT frontBufferWidth,
                         UINT frontBufferHeight,
                         bool hardwareTLOn,
                         D3DFORMAT optimalTextureFormat)
      : m_d3Device(d3Device),
      m_creationParams(creationParams),
      m_caps(caps),
      m_deviceLost(false),
      m_hardwareTLOn(hardwareTLOn),
      m_optimalTextureFormat(optimalTextureFormat)
{
   m_viewport.X = 0;
   m_viewport.Y = 0;
   m_viewport.Width = frontBufferWidth;
   m_viewport.Height = frontBufferHeight;
   m_viewport.MinZ = 0.0f;
   m_viewport.MaxZ = 1.0f;
}

/////////////////////////////////////////////////////////////////////////////

D3DRenderer::~D3DRenderer()
{
   if (m_d3Device != NULL)
   {
      m_d3Device->Release();
      m_d3Device = NULL;
   }
}

/////////////////////////////////////////////////////////////////////////////

void D3DRenderer::initRenderer()
{
   // Setup our D3D Device initial states
   m_d3Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
   m_d3Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
   m_d3Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
   m_d3Device->SetRenderState(D3DRS_LIGHTING, TRUE);
   m_d3Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
   m_d3Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

   m_d3Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
   m_d3Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
   m_d3Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

   m_d3Device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
   m_d3Device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
   m_d3Device->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

   m_d3Device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

   m_d3Device->SetRenderState(D3DRS_AMBIENT, 0x0D0D0D);
   m_d3Device->SetViewport(&m_viewport);
}

/////////////////////////////////////////////////////////////////////////////

void D3DRenderer::resetViewport(unsigned int width, unsigned int height)
{
   if ((m_d3Device) && (m_creationParams.Windowed == TRUE))
   {
      // Store new sizes
      m_creationParams.BackBufferWidth = width;
      m_creationParams.BackBufferHeight = height;

      m_viewport.X = 0;
      m_viewport.Y = 0;
      m_viewport.Width = width;
      m_viewport.Height = height;
      m_viewport.MinZ = 0.0f;
      m_viewport.MaxZ = 1.0f;

      // Reset the device
      m_d3Device->Reset(&m_creationParams);
      initRenderer();
   }
}

/////////////////////////////////////////////////////////////////////////////

void D3DRenderer::setViewMatrix(const D3DXMATRIX& mtx)
{
   m_d3Device->SetTransform(D3DTS_VIEW, &mtx);
}

/////////////////////////////////////////////////////////////////////////////

void D3DRenderer::setProjectionMatrix(const D3DXMATRIX& mtx)
{
   m_d3Device->SetTransform(D3DTS_PROJECTION, &mtx);
}

/////////////////////////////////////////////////////////////////////////////

void D3DRenderer::executeRenderingCommands(const std::list<RenderingCommand>& commands)
{
   m_d3Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF007FFF, 1.0f, 0);

   m_d3Device->BeginScene();

   for (std::list<RenderingCommand>::const_iterator it = commands.begin(); it != commands.end(); ++it)
   {
      (*it)();
   }

   m_d3Device->EndScene();
}

/////////////////////////////////////////////////////////////////////////////

void D3DRenderer::present()
{
   if (FAILED(m_d3Device->Present(NULL, NULL, NULL, NULL)))
   {
      m_deviceLost = true;
   }
}

/////////////////////////////////////////////////////////////////////////////

bool D3DRenderer::isGraphicsSystemReady() const
{
   return !(m_deviceLost == true);
}

/////////////////////////////////////////////////////////////////////////////

void D3DRenderer::attemptToRecoverGraphicsSystem()
{
   HRESULT deviceStatus = m_d3Device->TestCooperativeLevel();

   switch(deviceStatus)
   {
   case D3D_OK:
      {
         m_deviceLost = false;
         break;
      }

   case D3DERR_DEVICENOTRESET:
      {
         HRESULT resettingResult = m_d3Device->Reset(&m_creationParams);

         if (SUCCEEDED(resettingResult))
         {
            m_deviceLost = false;
         }
         break;
      }

   default:
      break;
   }
}

/////////////////////////////////////////////////////////////////////////////

UINT D3DRenderer::getMaxLightsCount() const
{
   return m_caps.MaxActiveLights;
}

/////////////////////////////////////////////////////////////////////////////

UINT D3DRenderer::getSimultaneouslyRenderedTexturesCount() const
{
   UINT count = m_caps.MaxSimultaneousTextures;
   if (count > m_caps.MaxTextureBlendStages) count = m_caps.MaxTextureBlendStages;

   return count;
}

/////////////////////////////////////////////////////////////////////////////
