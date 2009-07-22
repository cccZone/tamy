#pragma once

#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\ManagedTexture.h"
#include <string>
#include "LightReflectingPropertiesStub.h"
#include "MaterialOperationImplementationMock.h"
#include "StageTextureRendererStub.h"
#include "TransparencyEnablerStub.h"
#include "CoordinatesOperationMock.h"
#include "RendererStub.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include <vector>
#include <algorithm>
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

template<>
class TTextureImplDestructor<int>
{
public:
   void destroy(int* tex) {}
};

///////////////////////////////////////////////////////////////////////////////

class MaterialsFactoryMock : public GraphicalEntitiesFactory
{
private:
   static RendererStub s_renderer;
   MaterialOperationImplementationMock m_matOp;
   TransparencyEnablerStub m_transparencyEnabler;
   StageTextureRendererStub m_textureRenderer;

public:
   MaterialsFactoryMock() : GraphicalEntitiesFactory("", s_renderer) {}

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

   PostProcessEffectNode* createPostProcessEffectNode(const std::string& name, 
                                                      RenderingTechnique& technique)
   {
      return NULL;
   }

protected:
   GraphicalEntity* createGraphicalEntity(const std::string& name,
                                          const std::vector<LitVertex>& vertices,
                                          const std::list<Face<USHORT> >& faces,
                                          const std::vector<RenderingTechnique*>& techniques)
    {
       return NULL;
   }

   SkinnedGraphicalEntity* createSkinnedGraphicalEntity(const std::string& name,
                                                        const std::vector<LitVertex>& vertices,
                                                        const std::list<Face<USHORT> >& faces,
                                                        const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                        const std::vector<SkinBoneDefinition>& skinBones,
                                                        const std::vector<RenderingTechnique*>& techniques)
    {
       return NULL;
   }

   SkyBox* createSkyBox() 
   {
      return NULL;
   }

   Texture* loadTexture(const std::string& path, const std::string& fileName)
   {
      return new ManagedTexture(fileName, new TTextureImpl<int>());
   }

   Texture* createEmptyTexture()
   {
      return new ManagedTexture("", new TTextureImpl<int>());
   }

   Material* createMaterialImpl(const std::string& name, RenderingTargetsPolicy& policy, LightReflectingProperties* lrp)
   {
      return new Material(name, policy, lrp, m_matOp, m_matOp, m_transparencyEnabler);
   }

   MaterialStage* createMaterialStage(Texture& texture,
                                      MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                                      MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2,
                                      CoordsOpCode coordsOp)
   {
      return new MaterialStage(texture, 
                               new MaterialOperation(m_matOp, colorOp, colorArg1, colorArg2),
                               new MaterialOperation(m_matOp, alphaOp, alphaArg1, alphaArg2),
                               new CoordinatesOperationMock(coordsOp),
                               m_textureRenderer);
   }

   RenderingTarget* createDefaultRenderingTarget() {return NULL;}

   TextureRenderingTarget* createTextureRenderingTarget(const std::string& name) {return NULL;}

   GraphicalEffect* createEffectImpl(const std::string& name,
                                     RenderingTargetsPolicy& policy,
                                     bool isTransparent,
                                     EffectDataSource* dataSource)
   {
      return NULL;
   }
};

///////////////////////////////////////////////////////////////////////////////
