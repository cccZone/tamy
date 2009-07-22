#pragma once

#include "impl-DirectX\D3DAbstractGraphicalEntity.h"
#include "core-Renderer\GraphicalEntity.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <stdexcept>
#include <string>
#include "core-Renderer\Face.h"


///////////////////////////////////////////////////////////////////////////////

template<typename VertexStruct>
class D3DGraphicalEntity : public D3DAbstractGraphicalEntity<VertexStruct>,
                           public GraphicalEntity
{
public:
   D3DGraphicalEntity(const std::string& name,
                      IDirect3DDevice9& d3Device,
                      const std::vector<LitVertex>& vertices,
                      const std::list<Face<USHORT> >& faces,
                      const std::vector<RenderingTechnique*>& techniques)
         : D3DAbstractGraphicalEntity<VertexStruct>(d3Device, vertices, faces),
         GraphicalEntity(name, techniques)
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

   void render(const D3DXMATRIX& globalMtx, DWORD subset)
   {
      m_d3Device.SetTransform(D3DTS_WORLD, &globalMtx);
      m_mesh->DrawSubset(subset);
   }
};

///////////////////////////////////////////////////////////////////////////////

