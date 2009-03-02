#include "IWFMeshLoader.h"
#include "libIWF\iwfObjects.h"
#include "MeshDefinition.h"


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

void IWFMeshLoader::parseMesh(MeshDefinition& mesh, 
                              AnimationDefinition& animation, 
                              const std::string& name)
{
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
      unsigned int matIdx = getMaterialDefinition(tex, mat, mesh.materials);


      // geometry creation step 1: create faces
      addSurface(surface, mesh.vertices.size(), mesh.faces, matIdx);

      // geometry creation step 2: create vertices
      for (UINT i = 0; i < surface->VertexCount; i++)
      {
         mesh.vertices.push_back(LitVertex(surface->Vertices[i].x,
            surface->Vertices[i].y,
            surface->Vertices[i].z,
            1, 0, 0,
            surface->Vertices[i].Normal.x,
            surface->Vertices[i].Normal.y,
            surface->Vertices[i].Normal.z,
            surface->Vertices[i].TexCoords[0][0],
            surface->Vertices[i].TexCoords[0][1]));
      }

      D3DXMatrixIdentity(&(mesh.localMtx));
   }
}

///////////////////////////////////////////////////////////////////////////////

void IWFMeshLoader::addSurface(iwfSurface* surface, 
                               USHORT vertexStart, 
                               std::list<Face<USHORT> >& faces,
                               unsigned int matIdx)
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
               faces.push_back(Face<USHORT>((USHORT)surface->Indices[startIdx] + vertexStart,
                                            (USHORT)surface->Indices[startIdx + 1] + vertexStart,
                                            (USHORT)surface->Indices[startIdx + 2] + vertexStart,
                                            matIdx));
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
                  faces.push_back(Face<USHORT>((USHORT)surface->Indices[i] + vertexStart,
                                               (USHORT)surface->Indices[i + 1] + vertexStart,
                                               (USHORT)surface->Indices[i + 2] + vertexStart,
                                               matIdx));
               } // End if 'Even' triangle
               else
               {
                  faces.push_back(Face<USHORT>((USHORT)surface->Indices[i] + vertexStart,
                                               (USHORT)surface->Indices[i + 2] + vertexStart,
                                               (USHORT)surface->Indices[i + 1] + vertexStart,
                                               matIdx));
               } // End if 'Odd' triangle
            }

            break;
         }

      case INDICES_TRIFAN:
         {
            // Index in fan order.
            for (long i = 1; i < surface->VertexCount - 1; i++)
            {
               faces.push_back(Face<USHORT>((USHORT)surface->Indices[0] + vertexStart,
                                            (USHORT)surface->Indices[i] + vertexStart,
                                            (USHORT)surface->Indices[i + 1] + vertexStart,
                                            matIdx));
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
               faces.push_back(Face<USHORT>(startIdx + vertexStart,
                                            startIdx + 1 + vertexStart,
                                            startIdx + 2 + vertexStart,
                                            matIdx));
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
               faces.push_back(Face<USHORT>(i + vertexStart, 
                                             i + 1 + vertexStart, 
                                             i + 2 + vertexStart,
                                             matIdx));
            } // End if 'Even' triangle
            else
            {
               faces.push_back(Face<USHORT>(i + vertexStart, 
                                             i + 2 + vertexStart, 
                                             i + 1 + vertexStart,
                                             matIdx));
            } // End if 'Odd' triangle

         }

         break;

      case VERTICES_TRIFAN:

         // Index in fan order.
         for (USHORT i = 1; i < surface->VertexCount - 1; i++)
         {
            faces.push_back(Face<USHORT>(vertexStart, 
                                          i + vertexStart, 
                                          i + 1 + vertexStart,
                                          matIdx));
         }

         break;

      }
   }
}

///////////////////////////////////////////////////////////////////////////////

unsigned int IWFMeshLoader::getMaterialDefinition(iwfTexture* texture, iwfMaterial* material,
                                                  std::vector<MaterialDefinition>& materials)
{
   std::string newMaterialName = ((material != NULL) && (material->Name != NULL)) ? material->Name : "";
   std::string newTextureName  = ((texture != NULL) && (texture->Name != NULL)) ? texture->Name : "";
   // look for a smilar material first
   for (unsigned int matIdx = 0; matIdx < materials.size(); ++matIdx)
   {
      MaterialDefinition& mat = materials.at(matIdx);
      if ((mat.matName == newMaterialName) && (mat.texName == newTextureName))
      {
         return matIdx;
      }
   }

   // this is something we don't have yet - create a new material
   materials.push_back(MaterialDefinition());
   MaterialDefinition& def = materials.back();
   if (material != NULL)
   {
      def.matName = newMaterialName;
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

   return materials.size() - 1;
}

///////////////////////////////////////////////////////////////////////////////
