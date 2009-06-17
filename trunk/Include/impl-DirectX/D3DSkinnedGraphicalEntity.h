#pragma once

#include "impl-DirectX\D3DAbstractGraphicalEntity.h"
#include "core-Renderer\SkinnedGraphicalEntity.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <stdexcept>
#include <string>
#include "core-ResourceManagement\Face.h"
#include "core-ResourceManagement\MeshDefinition.h"


///////////////////////////////////////////////////////////////////////////////

template<typename VertexStruct>
class D3DSkinnedGraphicalEntity : public D3DAbstractGraphicalEntity<VertexStruct>,
                                  public SkinnedGraphicalEntity
{
public:
   D3DSkinnedGraphicalEntity(const std::string& name,
                             IDirect3DDevice9& d3Device,
                             const MeshDefinition& subMesh,
                             const std::vector<Material*>& registeredMaterials)
         : D3DAbstractGraphicalEntity<VertexStruct>(d3Device, subMesh),
         SkinnedGraphicalEntity(name, 
                                subMesh.skinBones,
                                subMesh.bonesInfluencingAttribute, 
                                registeredMaterials)
   {
      setBoundingSphereRadius(m_maxCoord);

      for (std::list<Face<USHORT> >::const_iterator faceIt = subMesh.faces.begin();
           faceIt != subMesh.faces.end(); ++faceIt)
      {
         addTriangle(Triangle(subMesh.vertices.at(faceIt->idx[0]).m_coords,
                              subMesh.vertices.at(faceIt->idx[1]).m_coords,
                              subMesh.vertices.at(faceIt->idx[2]).m_coords));
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
};

///////////////////////////////////////////////////////////////////////////////

