#include "impl-DirectX\D3DResourceManager.h"
#include "impl-DirectX\D3DInitializer.h"
#include "impl-DirectX\D3DRenderer.h"
#include "impl-DirectX\D3DGraphicalEntity.h"
#include "impl-DirectX\D3DSkinnedGraphicalEntity.h"
#include "impl-DirectX\D3DLight.h"
#include "impl-DirectX\D3DLightReflectingProperties.h"
#include "impl-DirectX\D3DTexture.h"
#include "impl-DirectX\D3DEmptyTexture.h"
#include "impl-DirectX\D3DSkyBox.h"
#include "impl-DirectX\D3DMaterial.h"
#include "impl-DirectX\XFileGraphicalEntityLoader.h"
#include "impl-DirectX\D3DColorOperationImplementation.h"
#include "impl-DirectX\D3DAlphaOperationImplementation.h"
#include "impl-DirectX\D3DParticleSystem.h"
#include <stdexcept>
#include <string>
#include <cassert>

#include "core-Renderer\BackgroundPass.h"
#include "core-Renderer\DrawingPass.h"

#include "impl-DirectX\OpenALSoundSystem.h"
#include "impl-DirectX\SoundInitializer.h"
#include "impl-DirectX\SoundDeviceInfo.h"
#include "impl-DirectX\OALSoundDevice.h"
#include "impl-DirectX\OALSoundListener.h"
#include "impl-DirectX\OALSound3D.h"


///////////////////////////////////////////////////////////////////////////////

D3DResourceManager::D3DResourceManager(const std::string& texturesDirPath,
                                       const std::string& meshesDirPath,
                                       HWND hWnd, bool windowed)
      : ResourceManager(texturesDirPath)
{
   // ---------------------- initialize rendering system ----------------------
   m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
   if (m_d3d9 == NULL)
   {
      throw std::logic_error(std::string("Cannot initialize DirectX library"));
   }

   m_d3dInitializer = new D3DInitializer(*m_d3d9, hWnd, *this);
   if (m_d3dInitializer == NULL)
   {
      m_d3d9->Release();
      m_d3d9 = NULL;
      throw std::logic_error(std::string("Failed to create a Direct3D initializer"));
   }

   D3DSettings d3DSettings;
   if (windowed)
   {
      d3DSettings = m_d3dInitializer->findBestWindowedMode();
   }
   else
   {
      D3DDISPLAYMODE matchMode;
      matchMode.Width = 1024;
      matchMode.Width = 768;
      matchMode.Format = D3DFMT_X8R8G8B8;
      d3DSettings = m_d3dInitializer->findBestFullscreenMode(matchMode);
   }
   m_renderer = m_d3dInitializer->createDisplay(d3DSettings, hWnd);
   m_renderer->addPass(new BackgroundPass());
   m_renderer->addPass(new DrawingPass());

   // create material operation implementations
   m_colorOpImpl = new D3DColorOperationImplementation(m_renderer->getD3Device());
   m_alphaOpImpl = new D3DAlphaOperationImplementation(m_renderer->getD3Device());

   // register file handlers
   registerFileGraphicalEntityLoader(new XFileGraphicalEntityLoader(m_renderer->getD3Device(), meshesDirPath));

   // ---------------------- initialize sound system ----------------------
   m_soundSystem = new OpenALSoundSystem();
   SoundInitializer soundInit(*m_soundSystem);

   SoundDeviceInfo idealDevice(OALV_UNKNOWN, 8);
   const SoundDeviceInfo& bestDevice = soundInit.findBest(idealDevice);
   m_soundDevice = m_soundSystem->createDevice(bestDevice);
}

///////////////////////////////////////////////////////////////////////////////

D3DResourceManager::~D3DResourceManager()
{
   delete m_soundDevice;
   m_soundDevice = NULL;

   delete m_soundSystem;
   m_soundSystem = NULL;

   delete m_renderer;
   m_renderer = NULL;

   delete m_d3dInitializer;
   m_d3dInitializer = NULL;

   m_d3d9->Release();
   m_d3d9 = NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool D3DResourceManager::checkDeviceCaps(const D3DCAPS9& caps)
{
   if(!(caps.FVFCaps & D3DFVFCAPS_PSIZE)) {return false;}
   if(caps.MaxPointSize <= 1.0f) {return false;}

   return true;
}

///////////////////////////////////////////////////////////////////////////////

Renderer& D3DResourceManager::getRendererInstance()
{
   return *m_renderer;
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntity* D3DResourceManager::createGraphicalEntity(const std::string& name,
                          const MeshDefinition& subMesh,
                          const std::vector<Material*>& registeredMaterials)
{
   return new D3DGraphicalEntity<LitVertex>(name, 
                                            m_renderer->getD3Device(), 
                                            subMesh,
                                            registeredMaterials);
}

///////////////////////////////////////////////////////////////////////////////

SkinnedGraphicalEntity* D3DResourceManager::createSkinedGraphicalEntity(const std::string& name,
                          const MeshDefinition& subMesh,
                          const std::vector<Material*>& registeredMaterials)
{
   return new D3DSkinnedGraphicalEntity<LitVertex>(name, 
                                       m_renderer->getD3Device(), 
                                       subMesh,
                                       registeredMaterials);
}

///////////////////////////////////////////////////////////////////////////////

Light* D3DResourceManager::createLightImpl(const std::string& name)
{
   return new D3DLight(name, m_renderer->getD3Device());
}

///////////////////////////////////////////////////////////////////////////////

LightReflectingProperties* D3DResourceManager::createLightReflectingProperties()
{
   return new D3DLightReflectingProperties(m_renderer->getD3Device());
}

///////////////////////////////////////////////////////////////////////////////

Texture* D3DResourceManager::createEmptyTexture()
{
   return new D3DEmptyTexture(m_renderer->getD3Device());
}

///////////////////////////////////////////////////////////////////////////////

Texture* D3DResourceManager::loadTexture(const std::string& textureDirPath, const std::string& name)
{
   IDirect3DTexture9* loadedTex = NULL;
   std::string fullPathToTexture = textureDirPath + std::string("\\") + name;

   HRESULT res = D3DXCreateTextureFromFileEx(&(m_renderer->getD3Device()),
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

   Texture* tex = new D3DTexture(name, m_renderer->getD3Device(), *loadedTex);
   loadedTex->Release();

   return tex;
}

///////////////////////////////////////////////////////////////////////////////

SkyBox* D3DResourceManager::createSkyBoxImpl()
{
   IDirect3DVertexBuffer9* vb = createVertexBuffer(sizeof(D3DSkyBoxVertex) * 24, 
                                                   D3DUSAGE_WRITEONLY,
                                                   D3DSkyBoxVertex::FVF,
                                                   D3DPOOL_MANAGED);

   D3DSkyBoxVertex* pVertex = NULL;
   if (FAILED(vb->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD)))
   {
      throw std::logic_error(
         std::string("Cannot lock a vertex buffer"));
   }

   // Front quad (remember all quads point inward)
   *pVertex++  = D3DSkyBoxVertex( -10.0f,  10.0f,  10.0f, 0.0f, 0.0f );
   *pVertex++  = D3DSkyBoxVertex(  10.0f,  10.0f,  10.0f, 1.0f, 0.0f );
   *pVertex++  = D3DSkyBoxVertex( -10.0f, -10.0f,  10.0f, 0.0f, 1.0f );
   *pVertex++  = D3DSkyBoxVertex(  10.0f, -10.0f,  10.0f, 1.0f, 1.0f );

   // Back Quad
   *pVertex++  = D3DSkyBoxVertex(  10.0f,  10.0f, -10.0f, 0.0f, 0.0f );
   *pVertex++  = D3DSkyBoxVertex( -10.0f,  10.0f, -10.0f, 1.0f, 0.0f );
   *pVertex++  = D3DSkyBoxVertex(  10.0f, -10.0f, -10.0f, 0.0f, 1.0f );
   *pVertex++  = D3DSkyBoxVertex( -10.0f, -10.0f, -10.0f, 1.0f, 1.0f );

   // Left Quad
   *pVertex++  = D3DSkyBoxVertex( -10.0f,  10.0f, -10.0f, 0.0f, 0.0f );
   *pVertex++  = D3DSkyBoxVertex( -10.0f,  10.0f,  10.0f, 1.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex( -10.0f, -10.0f, -10.0f, 0.0f, 1.0f );
   *pVertex++ = D3DSkyBoxVertex( -10.0f, -10.0f,  10.0f, 1.0f, 1.0f );

   // Right Quad
   *pVertex++ = D3DSkyBoxVertex(  10.0f,  10.0f,  10.0f, 0.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f,  10.0f, -10.0f, 1.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f, -10.0f,  10.0f, 0.0f, 1.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f, -10.0f, -10.0f, 1.0f, 1.0f );

   // Top Quad
   *pVertex++ = D3DSkyBoxVertex( -10.0f,  10.0f, -10.0f, 0.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f,  10.0f, -10.0f, 1.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex( -10.0f,  10.0f,  10.0f, 0.0f, 1.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f,  10.0f,  10.0f, 1.0f, 1.0f );

   // Bottom Quad
   *pVertex++ = D3DSkyBoxVertex( -10.0f, -10.0f,  10.0f, 0.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f, -10.0f,  10.0f, 1.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex( -10.0f, -10.0f, -10.0f, 0.0f, 1.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f, -10.0f, -10.0f, 1.0f, 1.0f );

   vb->Unlock();

   return new D3DSkyBox(m_renderer->getD3Device(), vb);
}

///////////////////////////////////////////////////////////////////////////////

Material* D3DResourceManager::createMaterialImpl(LightReflectingProperties& lrp, unsigned int index)
{
   return new D3DMaterial(m_renderer->getD3Device(), lrp, *m_alphaOpImpl, *m_colorOpImpl, index);
}

///////////////////////////////////////////////////////////////////////////////

MaterialOperationImplementation& D3DResourceManager::getColorOperationImpl()
{
   return *m_colorOpImpl;
}

///////////////////////////////////////////////////////////////////////////////

MaterialOperationImplementation& D3DResourceManager::getAlphaOperationImpl()
{
   return *m_alphaOpImpl;
}

///////////////////////////////////////////////////////////////////////////////

IDirect3DVertexBuffer9* D3DResourceManager::createVertexBuffer(UINT length, 
                                                               DWORD usageFlags, 
                                                               DWORD fvf, 
                                                               D3DPOOL memoryPool)
{
   assert(length > 0);

   if (!m_renderer->isHardwareTLOn()) usageFlags |= D3DUSAGE_SOFTWAREPROCESSING;

   IDirect3DVertexBuffer9* vertexBuffer = NULL;
   HRESULT res = m_renderer->getD3Device().CreateVertexBuffer(length,
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

IDirect3DIndexBuffer9* D3DResourceManager::createIndexBuffer(UINT length, 
                                                             DWORD usageFlags, 
                                                             D3DFORMAT format, 
                                                             D3DPOOL memoryPool)
{
   if (!m_renderer->isHardwareTLOn()) usageFlags |= D3DUSAGE_SOFTWAREPROCESSING;

   IDirect3DIndexBuffer9* indexBuffer = NULL;

   HRESULT res = m_renderer->getD3Device().CreateIndexBuffer(length,
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

SoundDevice& D3DResourceManager::getSoundDeviceInstance()
{
   return *m_soundDevice;
}

/////////////////////////////////////////////////////////////////////////////

SoundListener* D3DResourceManager::createSoundListener()
{
   return new OALSoundListener(*m_soundSystem);
}

/////////////////////////////////////////////////////////////////////////////

Sound3D* D3DResourceManager::createSound3D(const std::string& name, 
                                           bool dynamic, 
                                           Sound& sound,
                                           float hearingRadius)
{
   return new OALSound3D(*m_soundSystem, name, dynamic, sound, hearingRadius);
}

/////////////////////////////////////////////////////////////////////////////

ParticleSystem* D3DResourceManager::createParticleSystem(const std::string& name, 
                                                         bool isDynamic, 
                                                         Material& material,
                                                         unsigned int particlesCount)
{
   return new D3DParticleSystem(m_renderer->getD3Device(), 
                                name, 
                                isDynamic, 
                                material, 
                                particlesCount);
}

/////////////////////////////////////////////////////////////////////////////
