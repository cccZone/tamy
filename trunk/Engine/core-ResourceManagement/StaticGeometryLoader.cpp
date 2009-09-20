#include "core-ResourceManagement\StaticGeometryLoader.h"
#include "core-ResourceManagement\GraphicalDataSource.h"
#include "core-ResourceManagement\StaticGeometry.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\Material.h"
#include <sstream>
#include <deque>


///////////////////////////////////////////////////////////////////////////////

StaticGeometryLoader::StaticGeometryLoader(GraphicalEntitiesFactory& factory)
: m_factory(factory)
{
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometryLoader::load(ResourceStorage<Material>& materialsStorage,
                                GraphicalDataSource& source,
                                StaticGeometry& geometryStorage)
{
   MeshDefinition rootMesh;
   AnimationDefinition animation;
   std::vector<MaterialDefinition> materials;

   source.parseMesh(rootMesh, animation, materials, "staticGeometry");

   unsigned int materialsCount = materials.size();
   std::vector<LitVertex> vertices;
   std::vector<Face<USHORT> > faces;
   for (unsigned int matIdx = 0; matIdx < materialsCount; ++matIdx)
   {
      Material& material = createMaterial(materials[matIdx], 
                                          materialsStorage);

      vertices.clear();
      faces.clear();
      extractSubMeshes(materials[matIdx].matName, rootMesh, vertices, faces);
      geometryStorage.addMesh(vertices, faces, material);
   }
}

///////////////////////////////////////////////////////////////////////////////

Material& StaticGeometryLoader::createMaterial(MaterialDefinition& material,
                                               ResourceStorage<Material>& materialsStorage)
{
   if (materialsStorage.is(material.matName) == false)
   {
      LightReflectingProperties* lrp  = m_factory.createLightReflectingProperties();
      lrp->setAmbientColor(material.ambient);
      lrp->setDiffuseColor(material.diffuse);
      lrp->setSpecularColor(material.specular);
      lrp->setEmissiveColor(material.emissive);
      lrp->setPower(material.power);

      MaterialStage* stage = m_factory.createMaterialStage(material.texName,
                                                           material.colorOp, 
                                                           material.colorArg1, 
                                                           material.colorArg2,
                                                           material.alphaOp, 
                                                           material.alphaArg1, 
                                                           material.alphaArg2,
                                                           material.coordsOp);
      Material* mat = m_factory.createMaterial(material.matName, lrp);
      mat->addStage(stage);
      materialsStorage.add(mat);

      return *mat;
   }
   else
   {
      return materialsStorage.get(material.matName);
   }
}

//////////////////////////////////////////////////////////////////////////////

void StaticGeometryLoader::extractSubMeshes(const std::string& extractedMaterialName,
                                            MeshDefinition& inMesh, 
                                            std::vector<LitVertex>& outVertices,
                                            std::vector<Face<USHORT> >& outFaces)
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
         for (std::vector<Face<USHORT> >::iterator faceIt = currentMesh.faces.begin();
              faceIt != currentMesh.faces.end(); ++faceIt)
         {
            Face<USHORT>& inFace = *faceIt;
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
            outFaces.push_back(Face<USHORT> (idx[0], idx[1], idx[2], 0));
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
