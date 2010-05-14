#include "core-Renderer\TriangleMesh.h"
#include "core-Renderer\Renderer.h"
#include "core\Filesystem.h"
#include "core\SingletonsManager.h"
#include "core\ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( TriangleMesh, ttm, AM_BINARY )
   PROPERTY( "vertices", std::vector<LitVertex>, m_vertices )
   PROPERTY( "faces", std::vector<Face>, m_faces )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

TriangleMesh::TriangleMesh()
{
}

///////////////////////////////////////////////////////////////////////////////

TriangleMesh::TriangleMesh( const std::string& name,
                            const std::vector<LitVertex>& vertices,
                            const std::vector<Face>& faces )
: GeometryResource( name )
, m_vertices(vertices)
, m_faces(faces)
{
}

///////////////////////////////////////////////////////////////////////////////

void TriangleMesh::onResourceLoaded(ResourcesManager& mgr)
{
   Renderer& renderer = mgr.getInitializers().shared<Renderer>();
   renderer.implement<TriangleMesh> (*this);

   // calculate the bounding volume
   unsigned int verticesCount = m_vertices.size();
   for (unsigned int i = 0; i < verticesCount; ++i)
   {
      m_boundingVol.include(m_vertices[i].m_coords);
   }

   D3DXMatrixIdentity(&m_identityMtx);
}

///////////////////////////////////////////////////////////////////////////////

BoundingVolume* TriangleMesh::calculateBoundingVolume() const
{
   return m_boundingVol * m_identityMtx;
}

///////////////////////////////////////////////////////////////////////////////

VertexArray* TriangleMesh::getGenericVertexArray()
{
   TVertexArray<LitVertex>* array = new TVertexArray<LitVertex>();

   unsigned int vtxCount = m_vertices.size();
   for (unsigned int i = 0; i < vtxCount; ++i)
   {
      array->add(m_vertices[i]);
   }

   return array;

}
///////////////////////////////////////////////////////////////////////////////

void TriangleMesh::render()
{
   impl().render();
}

///////////////////////////////////////////////////////////////////////////////
