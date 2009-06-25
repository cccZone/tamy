#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "core\Node.h"
#include <sstream>
#include <deque>


///////////////////////////////////////////////////////////////////////////////

void GraphicalEntityLoader::load(const std::string& name,
                                 MeshDefinition& outMesh,
                                 AnimationDefinition &outAnimation) 
{
   parseMesh(outMesh, outAnimation, name);
   ensureMeshNames(outMesh);
   outMesh.name = name;
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
