#pragma once

#include "ResourceManager.h"
#include "NullLightReflectingProperties.h"
#include "TextureStub.h"
#include "GraphicalEntityMock.h"
#include "SkinnedGraphicalEntityMock.h"
#include "Skeleton.h"


//////////////////////////////////////////////////////////////////////////////

class ResourceManagerStub : public ResourceManager
{
public:
   ResourceManagerStub() : ResourceManager("") 
   {
   }

   Renderer& getRendererInstance() {return *(reinterpret_cast<Renderer*> (NULL));}

   GraphicalEntity* createGraphicalEntity(const std::string& name,
                                          const MeshDefinition& subMesh,
                                          const std::vector<Material*>& registeredMaterials)
   {
      return new GraphicalEntityMock(name, registeredMaterials);
   }

   SkinnedGraphicalEntity* createSkinedGraphicalEntity(const std::string& name,
                          const MeshDefinition& subMesh,
                          const std::vector<Material*>& registeredMaterials)
   {
      return new SkinnedGraphicalEntityMock(name, subMesh.skinBones, subMesh.bonesInfluencingAttribute, registeredMaterials);
   }

protected:
   Light* createLightImpl(const std::string& name) {return NULL;}

   LightReflectingProperties* createLightReflectingProperties() {return new NullLightReflectingProperties();}

   Texture* createEmptyTexture() {return new TextureStub();}

   Texture* loadTexture(const std::string& textureDirPath, const std::string& name) {return new TextureStub(name);}

   SkyBox* createSkyBoxImpl() {return NULL;}
};

//////////////////////////////////////////////////////////////////////////////
