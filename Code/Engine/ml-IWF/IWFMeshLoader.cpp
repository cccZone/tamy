#include "ml-IWF\IWFMeshLoader.h"
#include "ml-IWF\libIWF\iwfObjects.h"
#include "ml-IWF\MeshDefinition.h"


///////////////////////////////////////////////////////////////////////////////

IWFMeshLoader::IWFMeshLoader(iwfMesh* fileMesh, 
                             const std::vector<iwfTexture*>& textures, 
                             const std::vector<iwfMaterial*>& materials)
      : m_fileMesh(fileMesh),
      m_textures(textures),
      m_materials(materials)
{
}

///////////////////////////////////////////////////////////////////////////////

void IWFMeshLoader::parseMesh(std::vector<MeshDefinition>& meshes, 
                              const std::string& name)
{
   Vector minPos(10000000, 10000000, 10000000);
   Vector maxPos(-10000000, -10000000, -10000000);

   std::vector<MaterialDefinition> tmpMaterials;
   MeshDefinition* mesh = NULL;

   // let's parse the mesh and explode it into a group of engine laws abiding meshes
   for (ULONG surfaceIdx = 0; surfaceIdx < m_fileMesh->SurfaceCount; ++surfaceIdx)
   {
      iwfSurface* surface = m_fileMesh->Surfaces[surfaceIdx];

      // determine the indices of the material and a texture as we loaded them into our structures
      iwfTexture* tex = NULL;
      iwfMaterial* mat = NULL;
      bool usesMaterial = ((surface->Components & SCOMPONENT_MATERIALS) == SCOMPONENT_MATERIALS);
      bool usesTexture = ((surface->Components & SCOMPONENT_TEXTURES) == SCOMPONENT_TEXTURES);
      if (usesMaterial && (surface->ChannelCount > 0) && (surface->MaterialIndices[0] >= 0))
      {
         mat = m_materials.at(surface->MaterialIndices[0]);
      }
      if (usesTexture && (surface->ChannelCount > 0) && (surface->TextureIndices[0] >= 0))
      {
         tex = m_textures.at(surface->TextureIndices[0]);
      }

      // select proper mesh based on the material
      mesh = selectMeshByMaterial(tex, mat, meshes);

      // geometry creation step 1: create faces
      addSurface(surface, mesh->vertices.size(), mesh->faces);

      // geometry creation step 2: create vertices
      for (UINT i = 0; i < surface->VertexCount; i++)
      {
         Vector vertexPos(surface->Vertices[i].x,
                               surface->Vertices[i].y,
                               surface->Vertices[i].z);

         mesh->vertices.push_back(LitVertex(
            vertexPos.x,
            vertexPos.y,
            vertexPos.z,
            surface->Vertices[i].Normal.x,
            surface->Vertices[i].Normal.y,
            surface->Vertices[i].Normal.z,
            surface->Vertices[i].TexCoords[0][0],
            surface->Vertices[i].TexCoords[0][1]));

         if (vertexPos.x < minPos.x) minPos.x = vertexPos.x;
         if (vertexPos.y < minPos.y) minPos.y = vertexPos.y;
         if (vertexPos.z < minPos.z) minPos.z = vertexPos.z;

         if (vertexPos.x > maxPos.x) maxPos.x = vertexPos.x;
         if (vertexPos.y > maxPos.y) maxPos.y = vertexPos.y;
         if (vertexPos.z > maxPos.z) maxPos.z = vertexPos.z;
      }
   }

   // translate all vertices so that the mesh is centered around the global origin
   Vector meshTranslation(minPos.x + (maxPos.x - minPos.x) / 2.f,
                               minPos.y + (maxPos.y - minPos.y) / 2.f,
                               minPos.z + (maxPos.z - minPos.z) / 2.f);

   unsigned int meshesCount = meshes.size();
   for (unsigned int i = 0; i < meshesCount; ++i)
   {
      mesh = &(meshes[i]);
      for (std::vector<LitVertex>::iterator vertexIt = mesh->vertices.begin();
           vertexIt != mesh->vertices.end(); ++vertexIt)
      {
         vertexIt->m_coords.v[0] -= meshTranslation.x; 
         vertexIt->m_coords.v[1] -= meshTranslation.y; 
         vertexIt->m_coords.v[2] -= meshTranslation.z; 
      }

      mesh->localMtx.setTranslation( Vector( meshTranslation.x, meshTranslation.y, meshTranslation.z ) );
   }


   // set meshes names
   if ( meshesCount == 1 )
   {
      // set the mesh's name
      meshes[0].name = name;
   }
   else
   {
      char tmpName[512];
      for (unsigned int i = 0; i < meshesCount; ++i)
      {
         sprintf_s( tmpName, "%s_%d", name.c_str(), i );
         meshes[i].name = tmpName;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void IWFMeshLoader::addSurface(iwfSurface* surface, 
                               USHORT vertexStart, 
                               std::vector<Face >& faces)
{
   if (surface->IndexCount > 0)
   {
      ULONG indexType = surface->IndexFlags & INDICES_MASK_TYPE;

      // Interpret indices (we want them in tri-list format)
      switch (indexType)
      {
      case INDICES_TRILIST:
         {
            ULONG numFaces = surface->IndexCount / 3;
            for (ULONG i = 0; i < numFaces; i++) 
            {
               ULONG startIdx = i * 3;
               faces.push_back(Face((USHORT)surface->Indices[startIdx] + vertexStart,
                                            (USHORT)surface->Indices[startIdx + 1] + vertexStart,
                                            (USHORT)surface->Indices[startIdx + 2] + vertexStart));
            }
            break;
         }

      case INDICES_TRISTRIP:
         {
            // Index in strip order
            for (long i = 0; i < surface->IndexCount - 2; i++)
            {
               // Starting with triangle 0.
               // Is this an 'Odd' or 'Even' triangle
               if ((i % 2) == 0)
               {
                  faces.push_back(Face((USHORT)surface->Indices[i] + vertexStart,
                                               (USHORT)surface->Indices[i + 1] + vertexStart,
                                               (USHORT)surface->Indices[i + 2] + vertexStart));
               } // End if 'Even' triangle
               else
               {
                  faces.push_back(Face((USHORT)surface->Indices[i] + vertexStart,
                                               (USHORT)surface->Indices[i + 2] + vertexStart,
                                               (USHORT)surface->Indices[i + 1] + vertexStart));
               } // End if 'Odd' triangle
            }

            break;
         }

      case INDICES_TRIFAN:
         {
            // Index in fan order.
            for (long i = 1; i < surface->VertexCount - 1; i++)
            {
               faces.push_back(Face((USHORT)surface->Indices[0] + vertexStart,
                                            (USHORT)surface->Indices[i] + vertexStart,
                                            (USHORT)surface->Indices[i + 1] + vertexStart));
            }

            break;
         }
      }
   }
   else
   {
      ULONG vertexType = surface->VertexFlags & VERTICES_MASK_TYPE;

      // Interpret vertices (we want our indices in tri-list format)
      switch (vertexType)
      {
      case VERTICES_TRILIST:
         {
            // Straight fill
            USHORT numFaces = surface->VertexCount / 3;
            for (USHORT i = 0; i < numFaces; i++) 
            {
               USHORT startIdx = i * 3;
               faces.push_back(Face(startIdx + vertexStart,
                                            startIdx + 1 + vertexStart,
                                            startIdx + 2 + vertexStart));
            }
            break;
         }

      case VERTICES_TRISTRIP:

         // Index in strip order
         for (USHORT i = 0; i < surface->VertexCount - 2; i++)
         {
            // Starting with triangle 0.
            // Is this an 'Odd' or 'Even' triangle
            if ( (i % 2) == 0 )
            {
               faces.push_back(Face(i + vertexStart, 
                                             i + 1 + vertexStart, 
                                             i + 2 + vertexStart));
            } // End if 'Even' triangle
            else
            {
               faces.push_back(Face(i + vertexStart, 
                                             i + 2 + vertexStart, 
                                             i + 1 + vertexStart));
            } // End if 'Odd' triangle

         }

         break;

      case VERTICES_TRIFAN:

         // Index in fan order.
         for (USHORT i = 1; i < surface->VertexCount - 1; i++)
         {
            faces.push_back(Face(vertexStart, 
                                          i + vertexStart, 
                                          i + 1 + vertexStart));
         }

         break;

      }
   }
}

///////////////////////////////////////////////////////////////////////////////

MeshDefinition* IWFMeshLoader::selectMeshByMaterial(iwfTexture* texture, iwfMaterial* material,
                                                    std::vector<MeshDefinition>& meshes)
{
   std::string newMaterialName = ((material != NULL) && (material->Name != NULL)) ? material->Name : "";
   std::string newTextureName  = ((texture != NULL) && (texture->Name != NULL)) ? texture->Name : "";

   std::string matName = newMaterialName + std::string("_") + newTextureName;
   // look for a similar material first
   for (unsigned int meshIdx = 0; meshIdx < meshes.size(); ++meshIdx)
   {
      MaterialDefinition& mat = meshes.at(meshIdx).material;
      if (mat.matName == matName)
      {
         return &(meshes[meshIdx]);
      }
   }

   // this is something we don't have yet - create a new material
   meshes.push_back(MeshDefinition());
   MaterialDefinition& def = meshes.back().material;
   def.matName = matName;
   if (material != NULL)
   {
      def.ambient.r = material->Ambient.r;
      def.ambient.g = material->Ambient.g;
      def.ambient.b = material->Ambient.b;
      def.ambient.a = material->Ambient.a;

      def.diffuse.r = material->Diffuse.r;
      def.diffuse.g = material->Diffuse.g;
      def.diffuse.b = material->Diffuse.b;
      def.diffuse.a = material->Diffuse.a;

      def.specular.r = material->Specular.r;
      def.specular.g = material->Specular.g;
      def.specular.b = material->Specular.b;
      def.specular.a = material->Specular.a;

      def.emissive.r = material->Emissive.r;
      def.emissive.g = material->Emissive.g;
      def.emissive.b = material->Emissive.b;
      def.emissive.a = material->Emissive.a;

      def.power = material->Power;
   }

   if (texture != NULL)
   {
      def.texName = newTextureName;
   }

   return &(meshes.back());
}

///////////////////////////////////////////////////////////////////////////////
