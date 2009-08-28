#pragma once

#include "impl-DirectX\D3DAbstractGraphicalEntity.h"
#include "core-Renderer\SkinnedGraphicalEntity.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <stdexcept>
#include <string>
#include "core-Renderer\Face.h"


///////////////////////////////////////////////////////////////////////////////

template<typename VertexStruct>
class D3DSkinnedGraphicalEntity : public D3DAbstractGraphicalEntity<VertexStruct>,
                                  public SkinnedGraphicalEntity
{
public:
   D3DSkinnedGraphicalEntity(const std::string& name,
                             IDirect3DDevice9& d3Device,
                             const std::vector<LitVertex>& vertices,
                             const std::list<Face<USHORT> >& faces,
                             const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                             const std::vector<SkinBoneDefinition>& skinBones,
                             const std::vector<Material*>& materials)
         : D3DAbstractGraphicalEntity<VertexStruct>(d3Device, vertices, faces),
         SkinnedGraphicalEntity(name, 
                                skinBones,
                                bonesInfluencingAttribute, 
                                materials)
   {
      setBoundingSphereRadius(m_maxCoord);

      for (std::list<Face<USHORT> >::const_iterator faceIt = faces.begin();
           faceIt != faces.end(); ++faceIt)
      {
         addTriangle(Triangle(vertices.at(faceIt->idx[0]).m_coords,
                              vertices.at(faceIt->idx[1]).m_coords,
                              vertices.at(faceIt->idx[2]).m_coords));
      }
   }

   void render(DWORD subset)
   {
      m_mesh->DrawSubset(subset);
   }
};

///////////////////////////////////////////////////////////////////////////////

