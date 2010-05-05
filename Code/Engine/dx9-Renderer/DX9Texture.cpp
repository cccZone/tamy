#include "dx9-Renderer\DX9Texture.h"
#include "dx9-Renderer\DX9Renderer.h"


///////////////////////////////////////////////////////////////////////////////

DX9Texture::DX9Texture(Texture& texture)
: m_texture(texture)
, m_dxTexture(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

DX9Texture::~DX9Texture()
{
   if (m_dxTexture != NULL)
   {
      m_dxTexture->Release();
      m_dxTexture = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9Texture::initialize(Renderer& renderer)
{
   DX9Renderer* dxRenderer = dynamic_cast<DX9Renderer*> (&renderer);
   if (dxRenderer == NULL)
   {
      throw std::runtime_error("This implementation can work only with DX9Renderer");
   }

   // create the texture
   D3DFORMAT texFormat = dxRenderer->getOptimalTextureFormat();
   HRESULT res;

   byte* imageBuffer = NULL;
   unsigned int bufferSize = 0;
   m_texture.getBuffer(bufferSize, &imageBuffer);

   res = D3DXCreateTextureFromFileInMemoryEx(&(dxRenderer->getD3Device()),
      imageBuffer, bufferSize,
      D3DX_DEFAULT, D3DX_DEFAULT,
      D3DX_DEFAULT, 0, texFormat,
      D3DPOOL_MANAGED, 
      D3DX_DEFAULT, D3DX_DEFAULT,
      D3DCOLOR(), NULL, NULL, &m_dxTexture);
   m_texture.releaseData();

   if (FAILED(res))
   {
      std::string errorMsg = std::string("Can't load texture ") + m_texture.getTextureName();
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

void* DX9Texture::getPlatformSpecific() const
{
   return m_dxTexture;
}

///////////////////////////////////////////////////////////////////////////////
