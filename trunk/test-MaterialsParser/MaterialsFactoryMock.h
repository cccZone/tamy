#pragma once

#include "core-Renderer\GraphicalEntitiesFactory.h"
#include <string>
#include "LightReflectingPropertiesStub.h"
#include "MaterialOperationImplementationMock.h"
#include "TextureStub.h"
#include "TransparencyEnablerStub.h"
#include "core-Renderer\Material.h"
#include <vector>
#include <algorithm>
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

class MaterialsFactoryMock : public GraphicalEntitiesFactory
{
private:
   MaterialOperationImplementationMock m_matOp;
   TransparencyEnablerStub m_transparencyEnabler;

public:
   MaterialsFactoryMock() : GraphicalEntitiesFactory("") {}

   Light* createLight(const std::string& name)
   {
       return NULL;
   }

   LightReflectingProperties* createLightReflectingProperties() 
   {
      return new LightReflectingPropertiesStub();
   }

   ParticleSystem* createParticleSystem(const std::string& name, 
                                        bool isDynamic, 
                                        Material& material,
                                        unsigned int particlesCount)
   {
       return NULL;
   }

protected:
   GraphicalEntity* createGraphicalEntity(const std::string& name,
                                          const std::vector<LitVertex>& vertices,
                                          const std::list<Face<USHORT> >& faces,
                                          const std::vector<Material*>& registeredMaterials)
    {
       return NULL;
   }

   SkinnedGraphicalEntity* createSkinnedGraphicalEntity(const std::string& name,
                                                        const std::vector<LitVertex>& vertices,
                                                        const std::list<Face<USHORT> >& faces,
                                                        const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                        const std::vector<SkinBoneDefinition>& skinBones,
                                                        const std::vector<Material*>& registeredMaterials)
    {
       return NULL;
   }

   SkyBox* createSkyBox() 
   {
      return NULL;
   }

   Texture* loadTexture(const std::string& path, const std::string& fileName)
   {
      return new TextureStub(fileName);
   }

   Texture* createEmptyTexture()
   {
      return new TextureStub("");
   }

   Material* createMaterialImpl(const std::string& name, LightReflectingProperties* lrp)
   {
      return new Material(name, lrp, m_matOp, m_matOp, m_transparencyEnabler);
   }

   MaterialStage* createMaterialStage(Texture& texture,
                                      MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                                      MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2)
   {
      return new MaterialStage(texture, 
                               new MaterialOperation(m_matOp, colorOp, colorArg1, colorArg2),
                               new MaterialOperation(m_matOp, alphaOp, alphaArg1, alphaArg2));
   }
};

///////////////////////////////////////////////////////////////////////////////
