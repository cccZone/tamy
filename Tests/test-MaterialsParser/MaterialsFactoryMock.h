#pragma once

#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\ManagedTexture.h"
#include <string>
#include "RendererStub.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core\Filesystem.h"
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
   static Filesystem s_filesystem;

public:
   MaterialsFactoryMock() : GraphicalEntitiesFactory(s_filesystem, s_renderer) {}

   Light* createLight(const std::string& name)
   {
       return NULL;
   }

   ParticleSystem* createParticleSystem(const std::string& name, 
                                        Material& material,
                                        unsigned int particlesCount)
   {
       return NULL;
   }

   RendererImpl* createFixedRendererImpl()
   {
      return NULL;
   }

   GraphicalEffect* createEffect(const std::string& name)
   {
      return NULL;
   }

   SkyBoxSide* createSkyBoxSide(SkyBoxSideId side, Texture* tex) 
   {
      return NULL;
   }

   RenderingTargetsCleaner* createTargetsCleaner()
   {
      return NULL;
   }

   Font* createFont(const D3DXFONT_DESC& fontDesc)
   {
      return NULL;
   }

protected:
   GraphicalEntity* createGraphicalEntity(const std::string& name,
                                          const std::vector<LitVertex>& vertices,
                                          const std::vector<Face >& faces,
                                          const std::vector<Material*>& materials)
   {
      return NULL;
   }

   SkinnedGraphicalEntity* createSkinnedGraphicalEntity(const std::string& name,
                                                        const std::vector<LitVertex>& vertices,
                                                        const std::vector<Face >& faces,
                                                        const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                        const std::vector<SkinBoneDefinition>& skinBones,
                                                        const std::vector<Material*>& materials)
   {
      return NULL;
   }

   StaticGeometryRenderable* createStaticGeometry(Material& material,
                                                  const std::vector<LitVertex>& vertices,
                                                  const std::vector<Face >& faces)
   {
      return NULL;
   }

   Texture* loadTexture(const std::string& fileName)
   {
      return new ManagedTexture(fileName, new TTextureImpl<int>());
   }

   Texture* createEmptyTexture()
   {
      return new ManagedTexture("", new TTextureImpl<int>());
   }

   RenderingTarget* createDefaultRenderingTarget() {return NULL;}

   TextureRenderingTarget* createTextureRenderingTarget(const std::string& name) {return NULL;}

   PostProcessEffectRenderable* createPostProcessEffectRenderable(void)
   {
      return NULL;
   }
};

///////////////////////////////////////////////////////////////////////////////
