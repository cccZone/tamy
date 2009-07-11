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

///////////////////////////////////////////////////////////////////////////////

/**
 * This factory is responsible for creating all the graphical entities
 * used by the engine
 */
class GraphicalEntitiesFactory
{
private:
   std::string m_texturesPath;

   ResourceStorage<Texture> m_textures;

public:
   virtual ~GraphicalEntitiesFactory();

   virtual GraphicalEntity* createGraphicalEntity(const std::string& name,
                                                  const std::vector<LitVertex>& vertices,
                                                  const std::list<Face<USHORT> >& faces,
                                                  const std::vector<Material*>& registeredMaterials) = 0;

   virtual SkinnedGraphicalEntity* createSkinnedGraphicalEntity(const std::string& name,
                                                                const std::vector<LitVertex>& vertices,
                                                                const std::list<Face<USHORT> >& faces,
                                                                const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                                const std::vector<SkinBoneDefinition>& skinBones,
                                                                const std::vector<Material*>& registeredMaterials) = 0;


   virtual Light* createLight(const std::string& name) = 0;

   SkyBox* createSkyBox(Material& front, Material& back,
                        Material& left, Material& right,
                        Material& top, Material& bottom);

   virtual LightReflectingProperties* createLightReflectingProperties() = 0;

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

protected:
   GraphicalEntitiesFactory(const std::string& texturesPath);

   virtual SkyBox* createSkyBox() = 0;

   virtual Texture* loadTexture(const std::string& path, const std::string& fileName) = 0;
   virtual Texture* createEmptyTexture() = 0;

   virtual MaterialStage* createMaterialStage(Texture& tex,
                                              MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                                              MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2,
                                              CoordsOpCode coordsOperation) = 0;

   virtual Material* createMaterialImpl(const std::string& name,
                                        LightReflectingProperties* lrp) = 0;
};

///////////////////////////////////////////////////////////////////////////////

