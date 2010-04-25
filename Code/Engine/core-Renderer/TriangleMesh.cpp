#include "core-Renderer\TriangleMesh.h"
#include "core-Renderer\Renderer.h"
#include "core\Filesystem.h"
#include "core\SingletonsManager.h"
#include "core\ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_RTTI(TriangleMesh)
   PARENT(RendererObject)
END_RTTI

///////////////////////////////////////////////////////////////////////////////

TriangleMesh::TriangleMesh()
{
}

///////////////////////////////////////////////////////////////////////////////

TriangleMesh::TriangleMesh(const std::vector<LitVertex>& vertices,
                           const std::vector<Face>& faces)
: m_vertices(vertices)
, m_faces(faces)
{
   // calculate the bounding volume
   unsigned int verticesCount = m_vertices.size();
   for (unsigned int i = 0; i < verticesCount; ++i)
   {
      m_boundingVol.include(m_vertices[i].m_coords);
   }

   D3DXMatrixIdentity(&m_identityMtx);
}

///////////////////////////////////////////////////////////////////////////////

TriangleMesh::TriangleMesh(Filesystem& fs, 
                           const std::string& fileName)
{
   // TODO: wczytywanie z pliku
}

///////////////////////////////////////////////////////////////////////////////

void TriangleMesh::onLoaded(ResourcesManager& mgr)
{
   Renderer& renderer = mgr.getInitializers().shared<Renderer>();
   renderer.implement<TriangleMesh> (*this);
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
