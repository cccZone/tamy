#pragma once

#include "core-ResourceManagement\ResourceManager.h"
#include "impl-DirectX\GraphicalCapsEvaluator.h"
#include <windows.h>
#include <d3d9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class D3DInitializer;
class D3DRenderer;
class D3DMaterial;
class D3DColorOperationImplementation;
class D3DAlphaOperationImplementation;
class OpenALSoundSystem;
class OALSoundDevice;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is the implementation of the ResourceManager
 * for the implementation of the engine that uses the DirectX library
 */
class D3DResourceManager : public ResourceManager, public GraphicalCapsEvaluator
{
private:
   IDirect3D9* m_d3d9;
   D3DInitializer* m_d3dInitializer;
   D3DRenderer* m_renderer;
   bool m_hardwareTLOn;

   D3DColorOperationImplementation* m_colorOpImpl;
   D3DAlphaOperationImplementation* m_alphaOpImpl;

   OpenALSoundSystem* m_soundSystem;
   OALSoundDevice* m_soundDevice;

public:
   D3DResourceManager(const std::string& texturesDirPath,
                      const std::string& meshesDirPath,
                      HWND hWnd,
                      bool windowed);

   ~D3DResourceManager();

   bool checkDeviceCaps(const D3DCAPS9& caps);

   Renderer& getRendererInstance();

   GraphicalEntity* createGraphicalEntity(const std::string& name,
                                          const MeshDefinition& subMesh,
                                          const std::vector<Material*>& registeredMaterials);

   SkinnedGraphicalEntity* createSkinedGraphicalEntity(const std::string& name,
                          const MeshDefinition& subMesh,
                          const std::vector<Material*>& registeredMaterials);

   SoundDevice& getSoundDeviceInstance();

   SoundListener* createSoundListener();

   Sound3D* createSound3D(const std::string& name, bool dynamic, Sound& sound, float hearingRadius);

   ParticleSystem* createParticleSystem(const std::string& name, 
                                        bool isDynamic, 
                                        Material& material,
                                        unsigned int particlesCount);

protected:

   Light* createLightImpl(const std::string& name);

   LightReflectingProperties* createLightReflectingProperties();

   Texture* createEmptyTexture();

   Texture* loadTexture(const std::string& textureDirPath, const std::string& name);

   SkyBox* createSkyBoxImpl();

   Material* createMaterialImpl(LightReflectingProperties& lrp, unsigned int index);

   MaterialOperationImplementation& getColorOperationImpl();

   MaterialOperationImplementation& getAlphaOperationImpl();

private:
   IDirect3DVertexBuffer9* createVertexBuffer(UINT length, DWORD usageFlags, DWORD fvf, D3DPOOL memoryPool);
   IDirect3DIndexBuffer9* createIndexBuffer(UINT length, DWORD usageFlags, D3DFORMAT format, D3DPOOL memoryPool);
};

///////////////////////////////////////////////////////////////////////////////
