#include "core-Renderer\TriangleMesh.h"
#include "core-Renderer\RendererComponent.h"
#include "core\Filesystem.h"
#include "core\SingletonsManager.h"
#include "core\ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( TriangleMesh, GeometryResource, ttm, AM_BINARY )
   PROPERTY( std::vector<LitVertex>, m_vertices )
   PROPERTY( std::vector<Face>, m_faces )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

TriangleMesh::TriangleMesh()
{
   D3DXMatrixIdentity(&m_identityMtx);
}

///////////////////////////////////////////////////////////////////////////////

TriangleMesh::TriangleMesh( const std::string& name,
                            const std::vector<LitVertex>& vertices,
                            const std::vector<Face>& faces )
: GeometryResource( name )
, m_vertices(vertices)
, m_faces(faces)
{
   D3DXMatrixIdentity(&m_identityMtx);
}

///////////////////////////////////////////////////////////////////////////////

void TriangleMesh::onComponentAdded( Component< ResourcesManager >& component )
{
   RendererComponent* rendererComp = dynamic_cast< RendererComponent* >( &component );
   if ( rendererComp )
   {
      rendererComp->getRenderer().implement< TriangleMesh >( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void TriangleMesh::onComponentRemoved( Component< ResourcesManager >& component )
{
   RendererComponent* rendererComp = dynamic_cast< RendererComponent* >( &component );
   if ( rendererComp )
   {
      setImplementation( NULL );
   }
}

///////////////////////////////////////////////////////////////////////////////

void TriangleMesh::onResourceLoaded(ResourcesManager& mgr)
{
   unsigned int verticesCount = m_vertices.size();
   for (unsigned int i = 0; i < verticesCount; ++i)
   {
      m_boundingVol.include(m_vertices[i].m_coords);
   }
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
