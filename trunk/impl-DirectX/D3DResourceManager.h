#pragma once

#include "ResourceManager.h"
#include <windows.h>
#include <d3d9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class D3DInitializer;
class D3DRenderer;
class D3DMaterial;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is the implementation of the ResourceManager
 * for the implementation of the engine that uses the DirectX library
 */
class D3DResourceManager : public ResourceManager
{
private:
   IDirect3D9* m_d3d9;
   D3DInitializer* m_d3dInitializer;
   D3DRenderer* m_renderer;
   bool m_hardwareTLOn;

public:
   D3DResourceManager(const std::string& texturesDirPath,
                      const std::string& meshesDirPath,
                      HWND hWnd,
                      bool windowed);

   ~D3DResourceManager();

   Renderer& getRendererInstance();

   GraphicalEntity* createGraphicalEntity(const std::string& name,
                                          const MeshDefinition& subMesh,
                                          const std::vector<Material*>& registeredMaterials);

   SkinnedGraphicalEntity* createSkinedGraphicalEntity(const std::string& name,
                          const MeshDefinition& subMesh,
                          const std::vector<Material*>& registeredMaterials);

protected:

   Light* createLightImpl(const std::string& name);

   LightReflectingProperties* createLightReflectingProperties();

   Texture* createEmptyTexture();

   Texture* loadTexture(const std::string& textureDirPath, const std::string& name);

   SkyBox* createSkyBoxImpl();

   Material* createMaterial(Texture& emptyTexture, unsigned int index);

private:
   IDirect3DVertexBuffer9* createVertexBuffer(UINT length, DWORD usageFlags, DWORD fvf, D3DPOOL memoryPool);
   IDirect3DIndexBuffer9* createIndexBuffer(UINT length, DWORD usageFlags, D3DFORMAT format, D3DPOOL memoryPool);
};

///////////////////////////////////////////////////////////////////////////////
