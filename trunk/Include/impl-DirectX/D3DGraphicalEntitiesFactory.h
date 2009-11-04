#pragma once

#include "core-Renderer\GraphicalEntitiesFactory.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DRenderer;
class Filesystem;

///////////////////////////////////////////////////////////////////////////////

class D3DGraphicalEntitiesFactory : public GraphicalEntitiesFactory
{
private:
   IDirect3DDevice9& m_d3Device;
   D3DRenderer& m_renderer;

public:
   D3DGraphicalEntitiesFactory(Filesystem& filesystem,
                               IDirect3DDevice9& d3Device,
                               D3DRenderer& renderer);
   ~D3DGraphicalEntitiesFactory();

   ParticleSystem* createParticleSystem(const std::string& name, 
                                        Material& material,
                                        unsigned int particlesCount);

   SkyBoxSide* createSkyBoxSide(SkyBoxSideId side, Texture* tex);

   RenderingTarget* createDefaultRenderingTarget();

   TextureRenderingTarget* createTextureRenderingTarget(const std::string& name);

   RendererImpl* createFixedRendererImpl();

   GraphicalEffect* createEffect(const std::string& name);

   Font* createFont(const D3DXFONT_DESC& fontDesc);

   RenderingTargetsCleaner* createTargetsCleaner();

protected:
   GraphicalEntity* createGraphicalEntity(const std::string& name,
                                          const std::vector<LitVertex>& vertices,
                                          const std::vector<Face<USHORT> >& faces,
                                          const std::vector<Material*>& materials);

   SkinnedGraphicalEntity* createSkinnedGraphicalEntity(const std::string& name,
                                                        const std::vector<LitVertex>& vertices,
                                                        const std::vector<Face<USHORT> >& faces,
                                                        const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                        const std::vector<SkinBoneDefinition>& skinBones,
                                                        const std::vector<Material*>& materials);

   StaticGeometryRenderable* createStaticGeometry(Material& material,
                                                  const std::vector<LitVertex>& vertices,
                                                  const std::vector<Face<USHORT> >& faces);

   Texture* loadTexture(const std::string& fileName);
   Texture* createEmptyTexture();

   PostProcessEffectRenderable* createPostProcessEffectRenderable();
};

///////////////////////////////////////////////////////////////////////////////
