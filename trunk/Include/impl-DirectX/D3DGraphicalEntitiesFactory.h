#pragma once

#include "core-Renderer\GraphicalEntitiesFactory.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DRenderer;

///////////////////////////////////////////////////////////////////////////////

class D3DGraphicalEntitiesFactory : public GraphicalEntitiesFactory
{
private:
   IDirect3DDevice9& m_d3Device;
   D3DRenderer& m_renderer;

public:
   D3DGraphicalEntitiesFactory(const std::string& texturesPath,
                               IDirect3DDevice9& d3Device,
                               D3DRenderer& renderer);
   ~D3DGraphicalEntitiesFactory();

   Light* createLight(const std::string& name);

   ParticleSystem* createParticleSystem(const std::string& name, 
                                        bool isDynamic, 
                                        Material& material,
                                        unsigned int particlesCount);

   RenderingTarget* createDefaultRenderingTarget();

   TextureRenderingTarget* createTextureRenderingTarget(const std::string& name);

   SceneRenderingMechanism* createSceneRenderingMechanism(RenderingTargetsPolicy* policy);

protected:
   GraphicalEntity* createGraphicalEntity(const std::string& name,
                                          const std::vector<LitVertex>& vertices,
                                          const std::list<Face<USHORT> >& faces,
                                          const std::vector<Material*>& materials);

   SkinnedGraphicalEntity* createSkinnedGraphicalEntity(const std::string& name,
                                                        const std::vector<LitVertex>& vertices,
                                                        const std::list<Face<USHORT> >& faces,
                                                        const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                        const std::vector<SkinBoneDefinition>& skinBones,
                                                        const std::vector<Material*>& materials);

   SkyBox* createSkyBox();

   Texture* loadTexture(const std::string& path, const std::string& fileName);
   Texture* createEmptyTexture();

   GraphicalEffect* createEffectImpl(const std::string& name,
                                     RenderingTargetsPolicy& policy,
                                     EffectDataSource* dataSource);

   PostProcessEffectRenderable* createPostProcessEffectRenderable();
};

///////////////////////////////////////////////////////////////////////////////
