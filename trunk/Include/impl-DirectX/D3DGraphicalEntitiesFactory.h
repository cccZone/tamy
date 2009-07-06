#pragma once

#include "core-Renderer\GraphicalEntitiesFactory.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DRenderer;
class D3DColorOperationImplementation;
class D3DAlphaOperationImplementation;

///////////////////////////////////////////////////////////////////////////////

class D3DGraphicalEntitiesFactory : public GraphicalEntitiesFactory
{
private:
   IDirect3DDevice9& m_d3Device;
   D3DRenderer& m_renderer;
   D3DColorOperationImplementation* m_colorOpImpl;
   D3DAlphaOperationImplementation* m_alphaOpImpl;

public:
   D3DGraphicalEntitiesFactory(const std::string& texturesPath,
                               IDirect3DDevice9& d3Device,
                               D3DRenderer& renderer);
   ~D3DGraphicalEntitiesFactory();

   Light* createLight(const std::string& name);

   LightReflectingProperties* createLightReflectingProperties();

   ParticleSystem* createParticleSystem(const std::string& name, 
                                        bool isDynamic, 
                                        Material& material,
                                        unsigned int particlesCount);

protected:
   GraphicalEntity* createGraphicalEntity(const std::string& name,
                                          const std::vector<LitVertex>& vertices,
                                          const std::list<Face<USHORT> >& faces,
                                          const std::vector<Material*>& registeredMaterials);

   SkinnedGraphicalEntity* createSkinnedGraphicalEntity(const std::string& name,
                                                        const std::vector<LitVertex>& vertices,
                                                        const std::list<Face<USHORT> >& faces,
                                                        const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                        const std::vector<SkinBoneDefinition>& skinBones,
                                                        const std::vector<Material*>& registeredMaterials);

   SkyBox* createSkyBox();

   Texture* loadTexture(const std::string& path, const std::string& fileName);
   Texture* createEmptyTexture();

   MaterialStage* createMaterialStage(Texture& tex,
                                      MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                                      MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2);

   Material* createMaterial(const std::string& name,
                                    unsigned int index, 
                                    LightReflectingProperties* lrp);
};

///////////////////////////////////////////////////////////////////////////////
