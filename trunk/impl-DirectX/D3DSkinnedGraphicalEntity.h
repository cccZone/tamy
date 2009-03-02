#pragma once

#include "SkinnedGraphicalEntity.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <stdexcept>
#include <string>
#include "Face.h"
#include "MeshDefinition.h"


///////////////////////////////////////////////////////////////////////////////

template<typename VertexStruct>
class D3DSkinnedGraphicalEntity : public SkinnedGraphicalEntity
{
private:
   IDirect3DDevice9& m_d3Device;
   ID3DXMesh* m_mesh;

public:
   D3DSkinnedGraphicalEntity(const std::string& name,
                             IDirect3DDevice9& d3Device,
                             const MeshDefinition& subMesh,
                             const std::vector<Material*>& registeredMaterials)
                             : SkinnedGraphicalEntity(name, 
                                                      subMesh.skinBones,
                                                      subMesh.bonesInfluencingAttribute, 
                                                      registeredMaterials),
         m_d3Device(d3Device),
         m_mesh(NULL)
   {
      HRESULT res = D3DXCreateMeshFVF(subMesh.faces.size(), subMesh.vertices.size(), 
                                      D3DXMESH_MANAGED, VertexStruct::FVF, 
                                      &d3Device, &m_mesh);
      if (FAILED(res)) { throw std::logic_error(std::string("Can't create a mesh")); }

      // fill the vertex buffer
      VertexStruct* pVertex = NULL;
      res = m_mesh->LockVertexBuffer(D3DLOCK_DISCARD, (void**)&pVertex);
      if (FAILED(res)) { throw std::logic_error(std::string("Can't lock the mesh's vertex buffer")); }

      for (typename std::list<VertexStruct>::const_iterator it = subMesh.vertices.begin(); 
           it != subMesh.vertices.end(); ++it)
      {
         *pVertex++ = *it;
      }
      m_mesh->UnlockVertexBuffer();

      // fill the index buffer & the attributes table
      USHORT* pIndex = NULL;
      DWORD* pAttrib = NULL;
      res = m_mesh->LockIndexBuffer(D3DLOCK_DISCARD, (void**)&pIndex);
      if (FAILED(res)) { throw std::logic_error(std::string("Can't lock the mesh's index buffer")); }
      res = m_mesh->LockAttributeBuffer(D3DLOCK_DISCARD, &pAttrib);
      if (FAILED(res)) { throw std::logic_error(std::string("Can't lock the mesh's attributes buffer")); }

      for (std::list<Face<USHORT> >::const_iterator it = subMesh.faces.begin(); 
           it != subMesh.faces.end(); ++it)
      {
         const Face<USHORT>& face = *it;
         *pIndex++  = face.idx[0]; 
         *pIndex++  = face.idx[1]; 
         *pIndex++  = face.idx[2];
         *pAttrib++ = face.subsetID;
      }
      m_mesh->UnlockIndexBuffer();
      m_mesh->UnlockAttributeBuffer();

      if (subMesh.faces.size() > 2)
      {
         optimizeMesh();
      }
   }

   ~D3DSkinnedGraphicalEntity()
   {
      if (m_mesh != NULL)
      {
         m_mesh->Release();
         m_mesh = NULL;
      }
   }
   void render(const std::vector<D3DXMATRIX>& matrices, DWORD subset)
   {
      for (unsigned char i = 0; i < matrices.size(); ++i)
      {
         m_d3Device.SetTransform(D3DTS_WORLDMATRIX(i), &(matrices[i]));
      }

      unsigned char numMatrices = matrices.size() - 1;
      m_d3Device.SetRenderState(D3DRS_VERTEXBLEND, numMatrices);

      m_mesh->DrawSubset(subset);
      m_d3Device.SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
   }

private:
   void optimizeMesh()
   {
      DWORD *adjacency = new DWORD[m_mesh->GetNumFaces() * 3];
      m_mesh->GenerateAdjacency(0.001f, adjacency);
      m_mesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE, adjacency, NULL, NULL, NULL);
      delete [] adjacency;
   }
};

///////////////////////////////////////////////////////////////////////////////

