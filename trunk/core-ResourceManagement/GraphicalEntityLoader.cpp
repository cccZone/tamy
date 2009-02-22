#include "GraphicalEntityLoader.h"
#include "ResourceManager.h"
#include "GraphicalEntity.h"
#include "Node.h"
#include <sstream>
#include <deque>


///////////////////////////////////////////////////////////////////////////////

GraphicalEntity& GraphicalEntityLoader::load(ResourceManager& resourceManager, const std::string& name) 
{
   MeshDefinition mesh;
   AnimationDefinition animation;
   parseMesh(mesh, animation, name);

   GraphicalEntity* entity = createGraphicalEntity(resourceManager, mesh, name);
   Skeleton* skeleton = new Skeleton(animation, new Node(""));
   entity->attachSkeleton(skeleton);

   return *entity;
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntity* GraphicalEntityLoader::createGraphicalEntity(ResourceManager& resourceManager, 
                                                              MeshDefinition& mesh, 
                                                              const std::string& name)
{
   // register the meshes
   GraphicalEntity* entityRootNode = NULL;

   std::deque<std::pair<GraphicalEntity*, MeshDefinition*> > meshesQueue;
   meshesQueue.push_back(std::make_pair(reinterpret_cast<GraphicalEntity*> (NULL), &mesh));

   unsigned int subMeshNum = 0;
   while (meshesQueue.size() > 0)
   {
      std::stringstream subMeshName;
      subMeshName << name;
      if (subMeshNum > 0) {subMeshName << "_" << subMeshNum;}
      subMeshNum++;

      GraphicalEntity* parentEntity = meshesQueue.front().first;
      MeshDefinition& currentMesh = *(meshesQueue.front().second);
      meshesQueue.pop_front();

      // create a graphical entity based on the geometry definition
      GraphicalEntity* currentEntity = NULL;      
      if (resourceManager.isGraphicalEntityRegistered(subMeshName.str()) == true)
      {
         currentEntity = &resourceManager.getGraphicalEntity(subMeshName.str());
      }
      else
      {
         currentEntity = &resourceManager.createGraphicalEntity(subMeshName.str(),
                                                                currentMesh);
      }

      // attach the entity to its parent (if there's one)
      if (parentEntity != NULL)
      {
         parentEntity->addChild(*currentEntity);
      }

      // if it's the very first mesh we're parsing - store the pointer to it,
      // 'cause it's the one we'll be returning from this method
      if (entityRootNode == NULL)
      {
         entityRootNode = currentEntity;
      }

      // add the mesh's children to the queue
      for (std::list<MeshDefinition>::iterator childrenIt = currentMesh.children.begin();
           childrenIt != currentMesh.children.end(); ++childrenIt)
      {
         meshesQueue.push_back(std::make_pair(currentEntity, &(*childrenIt)));
      }
   }

   // verify the results
   if (entityRootNode == NULL)
   {
      throw std::logic_error(std::string("Invalid mesh ") + name);
   }

   return entityRootNode;
}

///////////////////////////////////////////////////////////////////////////////
