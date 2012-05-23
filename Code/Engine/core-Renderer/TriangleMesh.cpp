#include "core-Renderer\TriangleMesh.h"
#include "core-Renderer\Renderer.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE_NO_PARENT( TriangleMesh, ttm, AM_BINARY );
   PARENT( GeometryResource );
   PROPERTY( std::vector<LitVertex>, m_vertices );
   PROPERTY( std::vector<Face>, m_faces );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

TriangleMesh::TriangleMesh()
{
   m_identityMtx = Matrix::IDENTITY;
}

///////////////////////////////////////////////////////////////////////////////

TriangleMesh::TriangleMesh( const FilePath& resourceName, const std::vector<LitVertex>& vertices, const std::vector<Face>& faces )
   : GeometryResource( resourceName )
   , m_vertices(vertices)
   , m_faces(faces)
{
   m_identityMtx = Matrix::IDENTITY;
}

///////////////////////////////////////////////////////////////////////////////

TriangleMesh::~TriangleMesh()
{

}

///////////////////////////////////////////////////////////////////////////////

void TriangleMesh::onResourceLoaded( ResourcesManager& mgr )
{
   unsigned int verticesCount = m_vertices.size();
   for (unsigned int i = 0; i < verticesCount; ++i)
   {
      m_boundingVol.include( ( const Vector& )( m_vertices[i].m_coords ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

VertexArray* TriangleMesh::getGenericVertexArray() const
{
   TVertexArray<LitVertex>* array = new TVertexArray<LitVertex>();

   unsigned int vtxCount = m_vertices.size();
   for (unsigned int i = 0; i < vtxCount; ++i)
   {
      array->add( m_vertices[i] );
   }

   return array;

}

///////////////////////////////////////////////////////////////////////////////

void TriangleMesh::render( Renderer& renderer )
{
   new ( renderer() ) RCRenderTriangleMesh( *this );
}

///////////////////////////////////////////////////////////////////////////////
