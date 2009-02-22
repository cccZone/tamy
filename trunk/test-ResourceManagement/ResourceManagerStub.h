#pragma once

#include "ResourceManager.h"
#include "NullLightReflectingProperties.h"
#include "TextureStub.h"
#include "GraphicalEntityMock.h"
#include "Skeleton.h"


//////////////////////////////////////////////////////////////////////////////

class ResourceManagerStub : public ResourceManager
{
public:
   ResourceManagerStub() : ResourceManager("") {}

   Renderer& getRendererInstance() {return *(reinterpret_cast<Renderer*> (NULL));}

protected:
   GraphicalEntity* createGraphicalEntityImpl(const std::string& name,
                                              const MeshDefinition& subMesh,
                                              const std::vector<Material*>& registeredMaterials)
   {
      return new GraphicalEntityMock(name, registeredMaterials, subMesh.localMtx);
   }

   Light* createLightImpl(const std::string& name) {return NULL;}

   LightReflectingProperties* createLightReflectingProperties() {return new NullLightReflectingProperties();}

   Texture* createEmptyTexture() {return new TextureStub();}

   Texture* loadTexture(const std::string& textureDirPath, const std::string& name) {return new TextureStub(name);}

   SkyBox* createSkyBoxImpl() {return NULL;}
};

//////////////////////////////////////////////////////////////////////////////
