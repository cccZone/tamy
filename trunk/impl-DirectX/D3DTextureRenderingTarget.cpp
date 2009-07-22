#include "impl-DirectX\D3DTextureRenderingTarget.h"
#include "impl-DirectX\D3DRenderer.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

D3DTextureRenderingTarget::D3DTextureRenderingTarget(const std::string& name,
                                                     IDirect3DDevice9& d3Device,
                                                     D3DRenderer& renderer)
      : TextureRenderingTarget(name),
      m_d3Device(d3Device),
      m_renderer(renderer),
      m_texImpl(new TTextureImpl<IDirect3DTexture9>()),
      m_texture(NULL),
      m_surface(NULL)
{
   dynamic_cast<Subject<D3DRenderer, D3DGraphResourceOp>& > (m_renderer).attachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

D3DTextureRenderingTarget::~D3DTextureRenderingTarget()
{
   dynamic_cast<Subject<D3DRenderer, D3DGraphResourceOp>& > (m_renderer).detachObserver(*this);

   releaseTexture();

   delete m_texImpl;
   m_texImpl = NULL;
}

///////////////////////////////////////////////////////////////////////////////

TextureImpl& D3DTextureRenderingTarget::getImpl()
{
   return *m_texImpl;
}

///////////////////////////////////////////////////////////////////////////////

void D3DTextureRenderingTarget::use(unsigned char idx)
{
   if (m_surface == NULL) {return;}

   m_d3Device.SetRenderTarget(idx, m_surface);
}

///////////////////////////////////////////////////////////////////////////////

void D3DTextureRenderingTarget::update(D3DRenderer& renderer)
{
   releaseTexture();
   createTexture(renderer);
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
         createTexture(renderer);
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

      m_texImpl->set(NULL);
   }

   if (m_surface != NULL)
   {
      m_surface->Release();
      m_surface = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DTextureRenderingTarget::createTexture(D3DRenderer& renderer)
{
   if (m_texture != NULL)
   {
      throw std::runtime_error("Old texture exists");
   }

   HRESULT res = D3DXCreateTexture(&m_d3Device,
                                   renderer.getBackBufferWidth(),
                                   renderer.getBackBufferHeight(),
                                   1,
                                   D3DUSAGE_RENDERTARGET,
                                   renderer.getOptimalTextureFormat(),
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

   if (FAILED(m_texture->GetSurfaceLevel(0, &m_surface)) || (m_surface == NULL))
   {
      throw std::runtime_error("Can'r extract the surface from the rendering target texture");
   }


   m_texImpl->set(m_texture);
}

///////////////////////////////////////////////////////////////////////////////
