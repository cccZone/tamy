#pragma once

#include "core\ResourceStorage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include "core-Renderer\SkinBoneDefinition.h"
#include "core-Renderer\Face.h"
#include "core-Renderer\Texture.h"
#include <d3dx9.h>
#include <string>
#include <map>
#include <vector>
#include <list>


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntityLoader;
class AbstractGraphicalEntity;
class Light;
class SkyBoxSide;
class Material;
class MaterialStage;
class LightReflectingProperties;
class ResourceManager;
class GraphicalEntity;
class SkinnedGraphicalEntity;
class Texture;
class ParticleSystem;
struct MeshDefinition;
struct LitVertex;
class RenderingTarget;
class TextureRenderingTarget;
class Material;
class GraphicalEffect;
class PostProcessEffectController;
class AbstractGraphicalNode;
class Renderer;
class Camera;
class PostProcessEffectRenderable;
class RenderingTargetsPolicy;
class PostProcessMechanism;
class RendererImpl;
class PostProcessEffectController;
class RenderingTargetsCleaner;
class StaticGeometryRenderable;
class Font;
class Filesystem;

///////////////////////////////////////////////////////////////////////////////

enum SkyBoxSideId
{
   SBS_FRONT = 0,
   SBS_BACK = 1,
   SBS_LEFT = 2,
   SBS_RIGHT = 3,
   SBS_TOP = 4,
   SBS_BOTTOM  = 5
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This factory is responsible for creating all the graphical entities
 * used by the engine
 */
class GraphicalEntitiesFactory
{
protected:
   Filesystem& m_filesystem;

private:
   Renderer& m_renderer;
   ResourceStorage<Texture> m_textures;

public:
   virtual ~GraphicalEntitiesFactory();

   Camera* createCamera(const std::string& name);

   Light* createLight(const std::string& name);

   /**
    * Returns an instance of the built in textures storage
    */
   ResourceStorage<Texture>& getTexturesStorage() {return m_textures;}

   Texture* createTexture(const std::string& name);

   virtual GraphicalEntity* createGraphicalEntity(const std::string& name,
                                                  const std::vector<LitVertex>& vertices,
                                                  const std::vector<Face<USHORT> >& faces,
                                                  const std::vector<Material*>& materials) = 0;

   virtual SkinnedGraphicalEntity* createSkinnedGraphicalEntity(const std::string& name,
                                                                const std::vector<LitVertex>& vertices,
                                                                const std::vector<Face<USHORT> >& faces,
                                                                const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                                const std::vector<SkinBoneDefinition>& skinBones,
                                                                const std::vector<Material*>& materials) = 0;

   virtual StaticGeometryRenderable* createStaticGeometry(Material& material,
                                                          const std::vector<LitVertex>& vertices,
                                                          const std::vector<Face<USHORT> >& faces) = 0;

   virtual SkyBoxSide* createSkyBoxSide(SkyBoxSideId side, Texture* tex) = 0;

   LightReflectingProperties* createLightReflectingProperties();

   MaterialStage* createMaterialStage(const std::string& textureName,
                                      MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                                      MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2,
                                      CoordsOpCode coordsOperation);

   MaterialStage* createMaterialStage(Texture& texture,
                                      MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                                      MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2,
                                      CoordsOpCode coordsOperation);

   Material* createMaterial(const std::string& materialName,
                            LightReflectingProperties* lrp);

   virtual ParticleSystem* createParticleSystem(const std::string& name, 
                                                Material& material,
                                                unsigned int particlesCount) = 0;

   virtual RenderingTarget* createDefaultRenderingTarget() = 0;

   virtual TextureRenderingTarget* createTextureRenderingTarget(const std::string& name) = 0;

   virtual GraphicalEffect* createEffect(const std::string& name) = 0;

   virtual Font* createFont(const D3DXFONT_DESC& fontDesc) = 0;

   /**
    * This method will create a render implementation that uses the fixed
    * pipeline.
    *
    * @return  fixed pipeline renderer implementation
    */
   virtual RendererImpl* createFixedRendererImpl() = 0;

   /** 
    * This method creates a quad that can be used to render a full screen texture on.
    */
   virtual PostProcessEffectRenderable* createPostProcessEffectRenderable() = 0;

   /**
    * This method creates a utility that can clean rendering targets' contents.
    */
   virtual RenderingTargetsCleaner* createTargetsCleaner() = 0;

protected:
   GraphicalEntitiesFactory(Filesystem& filesystem, Renderer& renderer);

   virtual Texture* loadTexture(const std::string& fileName) = 0;
   virtual Texture* createEmptyTexture() = 0;
};

///////////////////////////////////////////////////////////////////////////////

