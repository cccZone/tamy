#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "core-ResourceManagement\GraphicalDataSource.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core\Node.h"
#include "core-Renderer\CompositeGraphicalEntity.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\SkinnedGraphicalEntity.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\Texture.h"
#include <sstream>
#include <deque>


///////////////////////////////////////////////////////////////////////////////

GraphicalEntityLoader::GraphicalEntityLoader(GraphicalEntitiesFactory& factory,
                                             ResourceStorage<Material>& materialsStorage)
      : m_factory(factory),
      m_materialsStorage(materialsStorage)
{
}

///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity* GraphicalEntityLoader::load(const std::string& name,
                                                     GraphicalDataSource& source) 
{
   MeshDefinition rootMesh;
   AnimationDefinition animation;
   std::vector<MaterialDefinition> materials;

   rootMesh.name = name;
   rootMesh.children.push_back(new MeshDefinition());

   source.parseMesh(*(rootMesh.children.back()), animation, materials, name);
   ensureMeshNames(rootMesh);

   createMaterials(materials);

   AbstractGraphicalEntity* entity = translateMeshToEntity(rootMesh);

   // set the animation
   entity->setAnimationDefinition(animation);

   return entity;
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEntityLoader::createMaterials(std::vector<MaterialDefinition>& materials)
{

   // register materials
   unsigned int materialsCount = materials.size();
   for (unsigned int matIdx = 0; matIdx < materialsCount; ++matIdx)
   {
      if (m_materialsStorage.is(materials[matIdx].matName) == false)
      {
         LightReflectingProperties* lrp  = m_factory.createLightReflectingProperties();
         lrp->setAmbientColor(materials[matIdx].ambient);
         lrp->setDiffuseColor(materials[matIdx].diffuse);
         lrp->setSpecularColor(materials[matIdx].specular);
         lrp->setEmissiveColor(materials[matIdx].emissive);
         lrp->setPower(materials[matIdx].power);

         MaterialStage* stage = m_factory.createMaterialStage(materials[matIdx].texName,
                                                              materials[matIdx].colorOp, 
                                                              materials[matIdx].colorArg1, 
                                                              materials[matIdx].colorArg2,
                                                              materials[matIdx].alphaOp, 
                                                              materials[matIdx].alphaArg1, 
                                                              materials[matIdx].alphaArg2);
         Material* mat = m_factory.createMaterial(materials[matIdx].matName, lrp);
         mat->addStage(stage);
         m_materialsStorage.add(mat);
      }
   }
}

//////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity* GraphicalEntityLoader::translateMeshToEntity(MeshDefinition& mesh)
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

      // locate the required materials
      std::vector<Material*> realMaterials;
      unsigned int materialsCount = currentMesh.materials.size();
      for (unsigned int matIdx = 0; matIdx < materialsCount; ++matIdx)
      {
         const std::string& matName = currentMesh.materials[matIdx];
         if (m_materialsStorage.is(matName))
         {
            realMaterials.push_back(&(m_materialsStorage.get(matName)));
         }
         else
         {
            throw std::runtime_error(std::string("Material ") + currentMesh.materials[matIdx] + " doesn't exist");
         }
      }

      // create a graphical entity based on the geometry definition
      if (currentMesh.faces.size() > 0)
      {
         AbstractGraphicalEntity* currentEntity = NULL;

         if (currentMesh.isSkin)
         {
            currentEntity = m_factory.createSkinnedGraphicalEntity(
                                                      currentMesh.name + "_entity",
                                                      currentMesh.vertices,
                                                      currentMesh.faces,
                                                      currentMesh.bonesInfluencingAttribute,
                                                      currentMesh.skinBones,
                                                      realMaterials);
         }
         else
         {
            currentEntity = m_factory.createGraphicalEntity(currentMesh.name + "_entity",
                                                            currentMesh.vertices,
                                                            currentMesh.faces,
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

   return entityRootNode;
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEntityLoader::ensureMeshNames(MeshDefinition& mesh)
{
   static unsigned long unnamedMeshIdx = 0;

   // register the meshes
   std::deque<MeshDefinition*> meshesQueue;
   meshesQueue.push_back(&mesh);

   while (meshesQueue.size() > 0)
   {
      MeshDefinition& currentMesh = *(meshesQueue.front());
      meshesQueue.pop_front();

      if (currentMesh.name.length() == 0)
      {
         std::stringstream str;
         str << "unnamedEntity_" << unnamedMeshIdx++;
         currentMesh.name = str.str();
      }

      // add the mesh's children to the queue
      for (std::list<MeshDefinition*>::iterator childrenIt = currentMesh.children.begin();
           childrenIt != currentMesh.children.end(); ++childrenIt)
      {
         meshesQueue.push_back(*childrenIt);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
