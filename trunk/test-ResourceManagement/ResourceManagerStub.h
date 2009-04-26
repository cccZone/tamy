#pragma once

#include "core-ResourceManagement\ResourceManager.h"
#include "core-Renderer\NullLightReflectingProperties.h"
#include "TextureStub.h"
#include "GraphicalEntityMock.h"
#include "SkinnedGraphicalEntityMock.h"
#include "core-Renderer\Skeleton.h"
#include "core-Renderer\Material.h"


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

   SoundDevice& getSoundDeviceInstance() {return *(reinterpret_cast<SoundDevice*> (NULL));}

   SoundListener* createSoundListener() {return NULL;}

   Sound3D* createSound3D(const std::string& name, 
                                  Sound& sound, 
                                  float hearingRadius)
   {
      return NULL;
   }

protected:
   Light* createLightImpl(const std::string& name) {return NULL;}

   LightReflectingProperties* createLightReflectingProperties() {return new NullLightReflectingProperties();}

   Texture* createEmptyTexture() {return new TextureStub();}

   Texture* loadTexture(const std::string& textureDirPath, const std::string& name) {return new TextureStub(name);}

   SkyBox* createSkyBoxImpl() {return NULL;}

   Material* createMaterial(Texture& emptyTexture, unsigned int index) {return new Material(emptyTexture, index);}
};

//////////////////////////////////////////////////////////////////////////////
