#include "core-ResourceManagement\TextureFactory.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "impl-DirectX\D3DTexture.h"
#include "impl-DirectX\D3DEmptyTexture.h"
#include "impl-DirectX\D3DRenderer.h"
#include <d3d9.h>
#include <string>


//////////////////////////////////////////////////////////////////////////////

Texture* TextureFactory<IDirect3D9>::load(const std::string& path, const std::string& fileName)
{
   IDirect3DTexture9* loadedTex = NULL;
   std::string fullPathToTexture = path + std::string("\\") + fileName;

   IDirect3DDevice9& d3Device = m_resMgr.shared<IDirect3DDevice9>();

   HRESULT res = D3DXCreateTextureFromFileEx(&d3Device,
                                             fullPathToTexture.c_str(),
                                             D3DX_DEFAULT, D3DX_DEFAULT,
                                             D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                             D3DPOOL_MANAGED, 
                                             D3DX_DEFAULT, D3DX_DEFAULT,
                                             D3DCOLOR(), NULL, NULL, &loadedTex);
   if (FAILED(res))
   {
      throw std::logic_error(std::string("Can't load texture from file ") + fullPathToTexture);
   }

   Texture* tex = new D3DTexture(fileName, d3Device, *loadedTex);
   loadedTex->Release();

   return tex;
}

//////////////////////////////////////////////////////////////////////////////

Texture* TextureFactory<IDirect3D9>::createEmpty()
{
   return new D3DEmptyTexture(m_resMgr.shared<IDirect3DDevice9>());
}

//////////////////////////////////////////////////////////////////////////////
