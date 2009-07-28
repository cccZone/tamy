#include "impl-DirectX\D3DRenderer.h"
#include <stdexcept>
#include <string>
#include <cassert>
#include "core-Renderer\GraphicalNode.h"
#include "impl-DirectX\D3DLight.h"


/////////////////////////////////////////////////////////////////////////////

D3DRenderer::D3DRenderer(IDirect3DDevice9* d3Device, 
                         D3DPRESENT_PARAMETERS creationParams,
                         D3DCAPS9 caps,
                         UINT frontBufferWidth,
                         UINT frontBufferHeight,
                         bool hardwareTLOn,
                         D3DFORMAT optimalTextureFormat)
      : Renderer(frontBufferWidth, frontBufferHeight),
      m_d3Device(d3Device),
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

   unsigned int maxTexStages = getSimultaneouslyRenderedTexturesCount();
   for (unsigned int stageIndex = 0; stageIndex < maxTexStages; ++stageIndex)
   {
      m_d3Device->SetSamplerState(stageIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
      m_d3Device->SetSamplerState(stageIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
      m_d3Device->SetSamplerState(stageIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
   }

   m_d3Device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

   m_d3Device->SetRenderState(D3DRS_AMBIENT, 0x0D0D0D);
   m_d3Device->SetViewport(&m_viewport);

   Subject<D3DRenderer, D3DGraphResourceOp>::notify(GRO_CREATE_RES);
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
      D3DPRESENT_PARAMETERS creationParams = m_creationParams;
      if (creationParams.Windowed) {creationParams.BackBufferFormat = D3DFMT_UNKNOWN;}
      creationParams.BackBufferCount = 0;
      creationParams.BackBufferWidth = 0;
      creationParams.BackBufferHeight = 0;

      Subject<D3DRenderer, D3DGraphResourceOp>::notify(GRO_RELEASE_RES);

      if (FAILED(m_d3Device->Reset(&creationParams)))
      {
         throw std::runtime_error("Could not reset the graphical device");
      }

      m_creationParams = creationParams;

      initRenderer();
   }
}

/////////////////////////////////////////////////////////////////////////////

void D3DRenderer::renderingBegin()
{
   m_d3Device->BeginScene();
}

/////////////////////////////////////////////////////////////////////////////

void D3DRenderer::cleanAllTargets(unsigned int count)
{
   for (unsigned int i = 0; i < count; ++i)
   {
      m_d3Device->Clear(i, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x0032C1FF, 1.0f, 0);
   }
}

/////////////////////////////////////////////////////////////////////////////

void D3DRenderer::renderingEnd()
{
   m_d3Device->EndScene();
   if (FAILED(m_d3Device->Present(NULL, NULL, NULL, NULL)))
   {
      m_deviceLost = true;
      return;
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

   if (m_deviceLost == false)
   {
      initRenderer();
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

IDirect3DVertexBuffer9* D3DRenderer::createVertexBuffer(UINT length, 
                                                        DWORD usageFlags, 
                                                        DWORD fvf, 
                                                        D3DPOOL memoryPool)
{
   assert(length > 0);

   if (!m_hardwareTLOn) usageFlags |= D3DUSAGE_SOFTWAREPROCESSING;

   IDirect3DVertexBuffer9* vertexBuffer = NULL;
   HRESULT res = m_d3Device->CreateVertexBuffer(length,
                                                usageFlags,
                                                fvf,
                                                memoryPool,
                                                &vertexBuffer,
                                                NULL);

   if (FAILED(res))
   {
      std::string errCode = "Cannot create a vertex buffer ";
      switch(res)
      {
      case D3DERR_INVALIDCALL:
         errCode += "- invalid parameters used";
         break;

      case D3DERR_OUTOFVIDEOMEMORY:
         errCode += "- not enough video memory left";
         break;

      case E_OUTOFMEMORY:
         errCode += "- not enough system memory left";
         break;

      default:
         errCode += "for an unknown reason";
         break;
      }
      throw std::logic_error(errCode);
   }

   return vertexBuffer;
}

/////////////////////////////////////////////////////////////////////////////

IDirect3DIndexBuffer9* D3DRenderer::createIndexBuffer(UINT length, 
                                                      DWORD usageFlags, 
                                                      D3DFORMAT format, 
                                                      D3DPOOL memoryPool)
{
   if (!m_hardwareTLOn) usageFlags |= D3DUSAGE_SOFTWAREPROCESSING;

   IDirect3DIndexBuffer9* indexBuffer = NULL;

   HRESULT res = m_d3Device->CreateIndexBuffer(length,
      usageFlags,
      format,
      memoryPool,
      &indexBuffer,
      NULL);

   if (FAILED(res))
   {
      throw std::logic_error(
         std::string("Cannot create an index buffer"));
   }

   return indexBuffer;
}

/////////////////////////////////////////////////////////////////////////////

bool D3DRenderer::wndProc(HWND hWnd, 
                          UINT message, 
                          WPARAM wParam, 
                          LPARAM lParam)
{
   switch (message)
   {
   case WM_SIZE: // fallthrough
   case WM_MOVE:
      {
         WINDOWINFO info;
         GetWindowInfo(hWnd, &info);

         resizeViewport(info.rcClient.right - info.rcClient.left, 
                        info.rcClient.bottom - info.rcClient.top, 
                        info.rcClient.left, 
                        info.rcClient.top,
                        info.rcClient.right, 
                        info.rcClient.bottom);

         break;
      }

   default:
      {
         return false;
      }
   }
   return true;
}

///////////////////////////////////////////////////////////////////////////////
