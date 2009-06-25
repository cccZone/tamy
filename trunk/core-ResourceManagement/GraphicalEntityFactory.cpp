#include "core-ResourceManagement\GraphicalEntityFactory.h"
#include "core-ResourceManagement\MeshDefinition.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "core-Renderer\CompositeGraphicalEntity.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\SkinnedGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "core-ResourceManagement\TextureFactory.h"
#include "core-ResourceManagement\LightReflectingPropertiesFactory.h"
#include "core-ResourceManagement\MaterialFactory.h"
#include "core-ResourceManagement\MaterialStageFactory.h"
#include "core-ResourceManagement\GraphicalEntityLoaderFactory.h"
#include <deque>


//////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity& 
Factory<AbstractGraphicalEntity>::operator()(const std::string& name)
{
   if (is(name))
   {
      return get(name);
   }
   else
   {
      GraphicalEntityLoader& loader = m_resMgr.resource<GraphicalEntityLoader>()(name);
      return (*this)(name, loader);
   }
}

//////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity& 
Factory<AbstractGraphicalEntity>::operator()(const std::string& name, 
                                             GraphicalEntityLoader& loader)
{
   MeshDefinition mesh;
   AnimationDefinition animation;
   loader.load(name, mesh, animation);

   AbstractGraphicalEntity& entity = (*this)(mesh);
   entity.setAnimationDefinition(animation);

   return entity;
}

//////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity&
Factory<AbstractGraphicalEntity>::operator()(MeshDefinition& mesh)
{
   // register the meshes
   CompositeGraphicalEntity* entityRootNode = new CompositeGraphicalEntity(mesh.name, mesh.localMtx);
   std::deque<std::pair<CompositeGraphicalEntity*, MeshDefinition*> > meshesQueue;
   meshesQueue.push_back(std::make_pair(entityRootNode, &mesh));

   while (meshesQueue.size() > 0)
   {
      CompositeGraphicalEntity* parentEntity = meshesQueue.front().first;
      MeshDefinition& currentMesh = *(meshesQueue.front().second);
      meshesQueue.pop_front();

      // create a graphical entity based on the geometry definition
      std::vector<Material*> realMaterials;
      getMaterials(currentMesh.materials, realMaterials);

      if (currentMesh.faces.size() > 0)
      {
         AbstractGraphicalEntity* currentEntity = NULL;

         if (currentMesh.isSkin)
         {
            currentEntity = createSkinedGraphicalEntity(
                                                      currentMesh.name + "_entity",
                                                      currentMesh,
                                                      realMaterials);
         }
         else
         {
            currentEntity = createGraphicalEntity(currentMesh.name + "_entity",
               currentMesh,
               realMaterials);
         }
         parentEntity->addChild(currentEntity);
      }

      // add the mesh's children to the queue
      for (std::list<MeshDefinition*>::iterator childrenIt = currentMesh.children.begin();
         childrenIt != currentMesh.children.end(); ++childrenIt)
      {
         CompositeGraphicalEntity* childNode = new CompositeGraphicalEntity((*childrenIt)->name, 
            (*childrenIt)->localMtx);
         parentEntity->addChild(childNode);
         meshesQueue.push_back(std::make_pair(childNode, *childrenIt));
      }
   }

   add(entityRootNode);

   return *entityRootNode;
}

//////////////////////////////////////////////////////////////////////////////

void Factory<AbstractGraphicalEntity>::getMaterials(const std::vector<MaterialDefinition>& inMaterialDefinitions,
                                                    std::vector<Material*>& outRealMaterials)
{
   // split the meshes by the materials
   outRealMaterials.resize(inMaterialDefinitions.size());
   for (unsigned int matIdx = 0; matIdx < inMaterialDefinitions.size(); ++matIdx)
   {
      const MaterialDefinition& mat = inMaterialDefinitions.at(matIdx);

      Material* realMat = NULL;
      if (m_resMgr.resource<Material>().is(mat.matName.c_str()) == true)
      {
         realMat = &(m_resMgr.resource<Material>().get(mat.matName.c_str()));
      }
      else
      {
         Texture& texture = m_resMgr.resource<Texture>()(mat.texName);
         LightReflectingProperties* lrp  = m_resMgr.resource<LightReflectingProperties>()();
         lrp->setAmbientColor(mat.ambient);
         lrp->setDiffuseColor(mat.diffuse);
         lrp->setSpecularColor(mat.specular);
         lrp->setEmissiveColor(mat.emissive);
         lrp->setPower(mat.power);
         realMat = &(m_resMgr.resource<Material>()(mat.matName.c_str(), lrp));
         MaterialStage* defaultStage = m_resMgr.resource<MaterialStage>()(texture,
                                                                       MOP_MULTIPLY, SC_LRP, SC_TEXTURE,
                                                                       MOP_DISABLE, SC_NONE, SC_NONE);
         realMat->addStage(defaultStage);
      }

      outRealMaterials[matIdx] = realMat;
   }
}

//////////////////////////////////////////////////////////////////////////////

