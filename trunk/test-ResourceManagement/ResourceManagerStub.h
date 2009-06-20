#pragma once

#include "core-ResourceManagement\ResourceManager.h"
#include "core-Renderer\NullLightReflectingProperties.h"
#include "TextureStub.h"
#include "GraphicalEntityMock.h"
#include "SkinnedGraphicalEntityMock.h"
#include "core-Renderer\Skeleton.h"
#include "core-Renderer\Material.h"
#include "MaterialOperationImplementationMock.h"
#include "core-Renderer\NullLightReflectingProperties.h"
#include "core-ResourceManagement\MeshDefinition.h"
#include "LightReflectingPropertiesStub.h"


//////////////////////////////////////////////////////////////////////////////

class ResourceManagerStub : public ResourceManager
{
private:
   MaterialOperationImplementationMock m_matOpImpl;

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
                          bool dynamic,
                          Sound& sound, 
                          float hearingRadius)
   {
      return NULL;
   }

   LightReflectingProperties* createLightReflectingProperties() {return new LightReflectingPropertiesStub();}

   ParticleSystem* createParticleSystem(const std::string& name, 
                                        bool isDynamic, 
                                        Material& material)
   {
      return NULL;
   }

protected:
   Light* createLightImpl(const std::string& name) {return NULL;}

   Texture* createEmptyTexture() {return new TextureStub();}

   Texture* loadTexture(const std::string& textureDirPath, const std::string& name) {return new TextureStub(name);}

   SkyBox* createSkyBoxImpl() {return NULL;}

   Material* createMaterialImpl(LightReflectingProperties& lrp, unsigned int index) {return new Material(lrp, m_matOpImpl, m_matOpImpl, index);}

   MaterialOperationImplementation& getColorOperationImpl() {return m_matOpImpl;}

   MaterialOperationImplementation& getAlphaOperationImpl() {return m_matOpImpl;}
};

//////////////////////////////////////////////////////////////////////////////
