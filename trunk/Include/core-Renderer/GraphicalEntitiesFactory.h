#pragma once

#include "core\ResourceStorage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include "core-Renderer\SkinBoneDefinition.h"
#include "core-Renderer\Face.h"
#include "core-Renderer\Texture.h"
#include <string>
#include <map>
#include <vector>
#include <list>


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntityLoader;
class AbstractGraphicalEntity;
class Light;
class SkyBox;
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
class EffectDataSource;
class AbstractGraphicalNode;
class Renderer;
class Camera;
class PostProcessEffectRenderable;
class RenderingTargetsPolicy;
class PostProcessMechanism;
class SceneRenderingMechanism;

///////////////////////////////////////////////////////////////////////////////

/**
 * This factory is responsible for creating all the graphical entities
 * used by the engine
 */
class GraphicalEntitiesFactory
{
private:
   std::string m_texturesPath;
   Renderer& m_renderer;
   ResourceStorage<Texture> m_textures;

public:
   virtual ~GraphicalEntitiesFactory();

   Camera* createCamera(const std::string& name);

   /**
    * Returns an instance of the built in textures storage
    */
   ResourceStorage<Texture>& getTexturesStorage() {return m_textures;}

   Texture* createTexture(const std::string& name);

   virtual GraphicalEntity* createGraphicalEntity(const std::string& name,
                                                  const std::vector<LitVertex>& vertices,
                                                  const std::list<Face<USHORT> >& faces,
                                                  const std::vector<Material*>& materials) = 0;

   virtual SkinnedGraphicalEntity* createSkinnedGraphicalEntity(const std::string& name,
                                                                const std::vector<LitVertex>& vertices,
                                                                const std::list<Face<USHORT> >& faces,
                                                                const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                                const std::vector<SkinBoneDefinition>& skinBones,
                                                                const std::vector<Material*>& materials) = 0;

   virtual Light* createLight(const std::string& name) = 0;

   SkyBox* createSkyBox(Texture& front, Texture& back,
                        Texture& left, Texture& right,
                        Texture& top, Texture& bottom);

   LightReflectingProperties* createLightReflectingProperties();

   MaterialStage* createMaterialStage(const std::string& textureName,
                                      MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                                      MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2,
                                      CoordsOpCode coordsOperation);

   Material* createMaterial(const std::string& materialName,
                            LightReflectingProperties* lrp);

   virtual ParticleSystem* createParticleSystem(const std::string& name, 
                                                bool isDynamic, 
                                                Material& material,
                                                unsigned int particlesCount) = 0;

   virtual RenderingTarget* createDefaultRenderingTarget() = 0;

   virtual TextureRenderingTarget* createTextureRenderingTarget(const std::string& name) = 0;

   GraphicalEffect* createEffect(const std::string& name, 
                                 bool isTransparent,
                                 EffectDataSource* dataSource);

   /**
    * This method will create a mechanism that allows to render the contents
    * of a single (and multiple) VisualSceneManager(s)
    */
   virtual SceneRenderingMechanism* createSceneRenderingMechanism(RenderingTargetsPolicy* policy) = 0;

   /**
    * This method will create a mechanism that allows to change
    * the appearance of a rendered scene
    */
   PostProcessMechanism* createPostProcessMechanism(RenderingTargetsPolicy* policy,
                                                    GraphicalEffect& effect);

protected:
   GraphicalEntitiesFactory(const std::string& texturesPath, Renderer& renderer);

   virtual SkyBox* createSkyBox() = 0;

   virtual Texture* loadTexture(const std::string& path, const std::string& fileName) = 0;
   virtual Texture* createEmptyTexture() = 0;

   virtual GraphicalEffect* createEffectImpl(const std::string& name, 
                                             RenderingTargetsPolicy& policy,
                                             bool isTransparent,
                                             EffectDataSource* dataSource) = 0;

   /** 
    * This method creates a quad that can be used to render a full screen texture on.
    */
   virtual PostProcessEffectRenderable* createPostProcessEffectRenderable() = 0;

};

///////////////////////////////////////////////////////////////////////////////

