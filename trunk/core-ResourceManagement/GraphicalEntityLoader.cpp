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
   entity->setAnimationDefinition(animation);

   return *entity;
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntity* GraphicalEntityLoader::createGraphicalEntity(ResourceManager& resourceManager, 
                                                              MeshDefinition& mesh, 
                                                              const std::string& name)
{
   static unsigned long unnamedMeshIdx = 0;

   // register the meshes
   GraphicalEntity* entityRootNode = NULL;

   std::deque<std::pair<GraphicalEntity*, MeshDefinition*> > meshesQueue;
   meshesQueue.push_back(std::make_pair(reinterpret_cast<GraphicalEntity*> (NULL), &mesh));

   while (meshesQueue.size() > 0)
   {
      GraphicalEntity* parentEntity = meshesQueue.front().first;
      MeshDefinition& currentMesh = *(meshesQueue.front().second);
      meshesQueue.pop_front();
      if (currentMesh.name.length() == 0)
      {
         std::stringstream str;
         str << "unnamedEntity_" << unnamedMeshIdx++;
         currentMesh.name = str.str();
      }

      // create a graphical entity based on the geometry definition
      GraphicalEntity* currentEntity = NULL;      
      if (resourceManager.isGraphicalEntityRegistered(currentMesh.name) == true)
      {
         currentEntity = &resourceManager.getGraphicalEntity(currentMesh.name);
      }
      else
      {
         currentEntity = &resourceManager.createGraphicalEntity(currentMesh.name,
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
