#include "core-ResourceManagement\MeshHierarchyFlattener.h"
#include "core-ResourceManagement\MeshDefinition.h"
#include <deque>


///////////////////////////////////////////////////////////////////////////////

void MeshHierarchyFlattener::operator()(MeshDefinition& hierarchicalMesh, 
                                        std::vector<FlatMesh*>& outMesh)
{
   unsigned int materialsCount = hierarchicalMesh.materials.size();
   for (unsigned int matIdx = 0; matIdx < materialsCount; ++matIdx)
   {
      FlatMesh* newMesh = new FlatMesh();
      outMesh.push_back(newMesh);
      newMesh->material = hierarchicalMesh.materials[matIdx];

      extractSubMeshes(newMesh->material, 
                       hierarchicalMesh, 
                       newMesh->vertices, 
                       newMesh->faces);
   }
}

///////////////////////////////////////////////////////////////////////////////

void MeshHierarchyFlattener::extractSubMeshes(const std::string& extractedMaterialName,
                                              MeshDefinition& inMesh, 
                                              std::vector<LitVertex>& outVertices,
                                              std::vector<Face>& outFaces)
{
   // register the meshes
   std::deque<std::pair<MeshDefinition*, D3DXMATRIX> > meshesQueue;
   D3DXMATRIX globalMtx;
   D3DXMatrixIdentity(&globalMtx);
   meshesQueue.push_back(std::make_pair(&inMesh, globalMtx));

   while (meshesQueue.size() > 0)
   {
      MeshDefinition& currentMesh = *(meshesQueue.front().first);
      globalMtx = currentMesh.localMtx * meshesQueue.front().second;
      meshesQueue.pop_front();

      // find the index that corresponds to the material we're looking for
      unsigned int materialIdx = -1;
      unsigned int materialsCount = currentMesh.materials.size();
      for (unsigned int i = 0; i < materialsCount; ++i)
      {
         if (currentMesh.materials[i] == extractedMaterialName)
         {
            materialIdx = i;
            break;
         }
      }

      // create a graphical entity based on the geometry definition
      if ((materialIdx != -1) && (currentMesh.faces.size() > 0))
      {
         std::vector<unsigned int> vertexRemap(currentMesh.vertices.size(), -1);

         int idx[3];
         for (std::vector<Face >::iterator faceIt = currentMesh.faces.begin();
              faceIt != currentMesh.faces.end(); ++faceIt)
         {
            Face& inFace = *faceIt;
            if (inFace.subsetID != materialIdx) {continue;}

            for (unsigned int j = 0; j < 3; ++j)
            {
               if (vertexRemap[inFace.idx[j]] == -1) 
               {
                  vertexRemap[inFace.idx[j]] = outVertices.size();

                  // transform the vertex to the global position
                  LitVertex transformedVtx = currentMesh.vertices[inFace.idx[j]];
                  D3DXVec3TransformCoord(&transformedVtx.m_coords, 
                                         &transformedVtx.m_coords, 
                                         &globalMtx);
                  D3DXVec3TransformNormal(&transformedVtx.m_normal, 
                                          &transformedVtx.m_normal, 
                                          &globalMtx);
                  outVertices.push_back(transformedVtx);
               }
               idx[j] = vertexRemap[inFace.idx[j]];
            }
            outFaces.push_back(Face (idx[0], idx[1], idx[2], 0));
         }
      }

      // add the mesh's children to the queue
      for (std::list<MeshDefinition*>::iterator childrenIt = currentMesh.children.begin();
         childrenIt != currentMesh.children.end(); ++childrenIt)
      {
         meshesQueue.push_back(std::make_pair(*childrenIt, globalMtx));
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
