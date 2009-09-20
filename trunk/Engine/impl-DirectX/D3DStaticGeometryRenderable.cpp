#include "impl-DirectX\D3DStaticGeometryRenderable.h"


///////////////////////////////////////////////////////////////////////////////

D3DStaticGeometryRenderable::D3DStaticGeometryRenderable(Material& material, 
                                                         const std::vector<LitVertex>& vertices,
                                                         const std::vector<Face<USHORT> >& faces,
                                                         IDirect3DDevice9& d3Device)
: StaticGeometryRenderable(material, vertices, faces)
, D3DAbstractGraphicalEntity(d3Device, vertices, faces)
, m_d3Device(d3Device)
{
}

///////////////////////////////////////////////////////////////////////////////

void D3DStaticGeometryRenderable::render()
{
   m_mesh->DrawSubset(0);
}

///////////////////////////////////////////////////////////////////////////////

StaticGeometryRenderable* D3DStaticGeometryRenderable::createSelf(
                                       Material& material,
                                       const std::vector<LitVertex>& vertices,
                                       const std::vector<Face<USHORT> >& faces)
{
   return new D3DStaticGeometryRenderable(material, 
                                          vertices, 
                                          faces, 
                                          m_d3Device);
}

///////////////////////////////////////////////////////////////////////////////
