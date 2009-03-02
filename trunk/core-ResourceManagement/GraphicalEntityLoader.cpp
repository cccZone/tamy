#include "GraphicalEntityLoader.h"
#include "ResourceManager.h"
#include "AbstractGraphicalEntity.h"
#include "GraphicalEntity.h"
#include "SkinnedGraphicalEntity.h"
#include "CompositeGraphicalEntity.h"
#include "Node.h"
#include <sstream>
#include <deque>


///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity& GraphicalEntityLoader::load(ResourceManager& resourceManager, 
                                                     const std::string& name) 
{
   MeshDefinition mesh;
   AnimationDefinition animation;
   parseMesh(mesh, animation, name);

   AbstractGraphicalEntity* entity = NULL;

   if (resourceManager.isGraphicalEntityRegistered(name) == true)
   {
      entity = &resourceManager.getGraphicalEntity(name);
   }
   else
   {
      ensureMeshNames(mesh);

      entity = createGraphicalEntity(resourceManager, mesh);
      entity->setAnimationDefinition(animation);

      resourceManager.registerGraphicalEntity(name, entity);
   }

   return *entity;
}

///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity* GraphicalEntityLoader::createGraphicalEntity(
                                 ResourceManager& resourceManager, 
                                 MeshDefinition& mesh)
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
      getMaterials(resourceManager, currentMesh.materials, realMaterials);

      if (currentMesh.faces.size() > 0)
      {
         AbstractGraphicalEntity* currentEntity = NULL;

         if (currentMesh.isSkin)
         {
            currentEntity = resourceManager.createSkinedGraphicalEntity(
                                          currentMesh.name + "_entity",
                                          currentMesh,
                                          realMaterials);
         }
         else
         {
            currentEntity = resourceManager.createGraphicalEntity(currentMesh.name + "_entity",
                                                                  currentMesh,
                                                                  realMaterials);
         }
         parentEntity->addChild(currentEntity);
      }

      // add the mesh's children to the queue
      for (std::list<MeshDefinition>::iterator childrenIt = currentMesh.children.begin();
           childrenIt != currentMesh.children.end(); ++childrenIt)
      {
         CompositeGraphicalEntity* childNode = new CompositeGraphicalEntity(childrenIt->name, 
                                                                            childrenIt->localMtx);
         parentEntity->addChild(childNode);
         meshesQueue.push_back(std::make_pair(childNode, &(*childrenIt)));
      }
   }


   return entityRootNode;
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEntityLoader::getMaterials(
                  ResourceManager& resourceManager,
                  const std::vector<MaterialDefinition>& inMaterialDefinitions,
                  std::vector<Material*>& outRealMaterials)
{
   // split the meshes by the materials
   outRealMaterials.resize(inMaterialDefinitions.size());
   for (unsigned int matIdx = 0; matIdx < inMaterialDefinitions.size(); ++matIdx)
   {
      const MaterialDefinition& mat = inMaterialDefinitions.at(matIdx);
      unsigned int id = resourceManager.addMaterial(mat.texName, mat.ambient, 
                                                    mat.diffuse, mat.specular, 
                                                    mat.emissive, mat.power);
      outRealMaterials[matIdx] = &(resourceManager.getMaterial(id));
   }
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
      for (std::list<MeshDefinition>::iterator childrenIt = currentMesh.children.begin();
           childrenIt != currentMesh.children.end(); ++childrenIt)
      {
         meshesQueue.push_back(&(*childrenIt));
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
