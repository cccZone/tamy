#include "impl-DirectX\D3DTextureRenderingTarget.h"
#include "impl-DirectX\D3DRenderer.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

D3DTextureRenderingTarget::D3DTextureRenderingTarget(const std::string& name,
                                                     unsigned int width,
                                                     unsigned int height,
                                                     unsigned int mipLevels,
                                                     IDirect3DDevice9& d3Device,
                                                     D3DRenderer& renderer)
      : TextureRenderingTarget(name),
      m_width(width),
      m_height(height),
      m_mipLevels(mipLevels),
      m_d3Device(d3Device),
      m_renderer(renderer),
      m_texture(NULL),
      m_surface(NULL)
{
   m_renderer.attachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

D3DTextureRenderingTarget::~D3DTextureRenderingTarget()
{
   m_renderer.detachObserver(*this);

   releaseTexture();
}

///////////////////////////////////////////////////////////////////////////////

void D3DTextureRenderingTarget::use(unsigned char idx)
{
   if (m_surface == NULL) {return;}

   m_d3Device.SetRenderTarget(idx, m_surface);
   m_d3Device.Clear(idx, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
}

///////////////////////////////////////////////////////////////////////////////

void D3DTextureRenderingTarget::setForRendering(unsigned char stageIdx)
{
   m_d3Device.SetTexture(stageIdx, m_texture);
}

///////////////////////////////////////////////////////////////////////////////

void D3DTextureRenderingTarget::update(D3DRenderer& renderer)
{
   releaseTexture();
   createTexture();
}

///////////////////////////////////////////////////////////////////////////////

void D3DTextureRenderingTarget::update(D3DRenderer& renderer, const D3DGraphResourceOp& operation)
{
   switch(operation)
   {
   case GRO_RELEASE_RES:
      {
         releaseTexture();
         break;
      }
   
   case GRO_CREATE_RES:
      {
         releaseTexture();
         createTexture();
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DTextureRenderingTarget::releaseTexture()
{
   if (m_texture != NULL)
   {
      m_texture->Release();
      m_texture = NULL;
   }

   if (m_surface != NULL)
   {
      m_surface->Release();
      m_surface = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DTextureRenderingTarget::createTexture()
{
   if (m_texture != NULL)
   {
      throw std::runtime_error("Old texture exists");
   }

   HRESULT res = D3DXCreateTexture(&m_d3Device,
                                   m_width,
                                   m_height,
                                   m_mipLevels,
                                   D3DUSAGE_RENDERTARGET,
                                   m_renderer.getOptimalTextureFormat(),
                                   D3DPOOL_DEFAULT,
                                   &m_texture);

   if (FAILED(res))
   {
      std::string errorMsg = std::string("Can't create a texture ");
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

}

///////////////////////////////////////////////////////////////////////////////
