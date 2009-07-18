#include "impl-DirectX\D3DRenderer.h"
#include <stdexcept>
#include <string>
#include <cassert>
#include "core-Renderer\GraphicalNode.h"
#include "impl-DirectX\D3DLight.h"
#include "core-Renderer\RenderingCommand.h"


/////////////////////////////////////////////////////////////////////////////

D3DRenderer::D3DRenderer(IDirect3DDevice9* d3Device, 
                         D3DPRESENT_PARAMETERS creationParams,
                         D3DCAPS9 caps,
                         UINT frontBufferWidth,
                         UINT frontBufferHeight,
                         bool hardwareTLOn,
                         D3DFORMAT optimalTextureFormat)
      : Renderer(caps.NumSimultaneousRTs),
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

   notify(GRO_CREATE_RES);
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

      notify(GRO_RELEASE_RES);

      if (FAILED(m_d3Device->Reset(&creationParams)))
      {
         throw std::runtime_error("Could not reset the graphical device");
      }

      m_creationParams = creationParams;

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

void D3DRenderer::renderingBegin()
{
   m_d3Device->BeginScene();
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
      throw std::logic_error(
         std::string("Cannot create a vertex buffer"));
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


/*
/////////////////////////////////////////////////////////////////////////////

const DWORD D3DRenderer::SCREEN_VERTEX::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;


private:
   struct SCREEN_VERTEX
   {
      D3DXVECTOR4 pos;
      DWORD clr;
      D3DXVECTOR2 tex1;

      static const DWORD FVF;
   };


// swap chain
   IDirect3DTexture9* m_renderTargetTexture;
   IDirect3DSurface9* m_renderTargetSurface;

   SCREEN_VERTEX m_fullScreenQuad[4];

void D3DRenderer::clearSwapChain()
{
   if (m_renderTargetSurface != NULL)
   {
      m_renderTargetSurface->Release();
      m_renderTargetSurface = NULL;
   }

   if (m_renderTargetTexture != NULL)
   {
      m_renderTargetTexture->Release();
      m_renderTargetTexture = NULL;
   }
}

/////////////////////////////////////////////////////////////////////////////

void D3DRenderer::createSwapChain()
{
   clearSwapChain();

   // create the rendering swap chain
   HRESULT res = D3DXCreateTexture(m_d3Device, 
                                   m_creationParams.BackBufferWidth, 
                                   m_creationParams.BackBufferHeight,
                                   1,
                                   D3DUSAGE_RENDERTARGET,
                                   m_creationParams.BackBufferFormat,
                                   D3DPOOL_DEFAULT,
                                   &m_renderTargetTexture);
   if (FAILED(res) || (m_renderTargetTexture == NULL))
   {
      std::string errorMsg = std::string("Texture creation failed");
      switch(res)
      {
         case D3DERR_INVALIDCALL:
            {
               errorMsg += " due to invalid parameters passed";
               break;
            }

         case D3DERR_NOTAVAILABLE:
            {
               errorMsg += " - this format is unavailable";
               break;
            }

         case D3DERR_OUTOFVIDEOMEMORY:
            {
               errorMsg += " due to the lack of video memory";
               break;
            }

         case D3DXERR_INVALIDDATA:
            {
               errorMsg += " due to invalid data";
               break;
            }

         case E_OUTOFMEMORY:
            {
               errorMsg += " due to the lack of system memory";
               break;
            }

         default:
            {
               errorMsg += " for unknown reason";
               break;
            }
      }
      throw std::logic_error(errorMsg);
   }

   if (FAILED(m_renderTargetTexture->GetSurfaceLevel(0, &m_renderTargetSurface)) || (m_renderTargetSurface == NULL))
   {
      throw std::runtime_error("Can'r extract the surface from a rendering target texture");
   }
}

/////////////////////////////////////////////////////////////////////////////

void D3DRenderer::prepareFullScreenQuad()
{
   D3DSURFACE_DESC desc;
   m_backBuffer->GetDesc(&desc);

   float width = (float)desc.Width - 0.5f;
   float height = (float)desc.Height - 0.5f;

   float texWidth = 1.f;
   float texHeight = 1.f;

   // fill in the vertex values
   m_fullScreenQuad[0].pos = D3DXVECTOR4(width, -0.5f, 0.0f, 1.0f);
   m_fullScreenQuad[0].clr = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.66666f);
   m_fullScreenQuad[0].tex1 = D3DXVECTOR2(texWidth, 0.0f);

   m_fullScreenQuad[1].pos = D3DXVECTOR4( width, height, 0.0f, 1.0f);
   m_fullScreenQuad[1].clr = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.66666f);
   m_fullScreenQuad[1].tex1 = D3DXVECTOR2(texWidth, texHeight);

   m_fullScreenQuad[2].pos = D3DXVECTOR4(-0.5f, -0.5f, 0.0f, 1.0f);
   m_fullScreenQuad[2].clr = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.66666f);
   m_fullScreenQuad[2].tex1 = D3DXVECTOR2(0.0f, 0.0f);

   m_fullScreenQuad[3].pos = D3DXVECTOR4(-0.5f, height, 0.0f, 1.0f);
   m_fullScreenQuad[3].clr = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.66666f);
   m_fullScreenQuad[3].tex1 = D3DXVECTOR2(0.0f, texHeight);
}

/////////////////////////////////////////////////////////////////////////////
*/