#include "core-Renderer\TriangleMesh.h"
#include "core-Renderer\Renderer.h"
#include "core.h"


// <geometry.todo> !!!!!!!!!!!!!!!!!!
// - At this point it's the vertex type that specifies the vertex description used,
// but the engine associates vertex description with a vertex shader, which is wrong.
// Make geometry define vertex description it's using!
//
// - Some geometries use arbitrary vertex formats ( FVF formats, i.e. debug geometry ) - make sure those will work after this change is implemented
//

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
   calculateBoundingVolume();
}

///////////////////////////////////////////////////////////////////////////////

TriangleMesh::~TriangleMesh()
{

}

///////////////////////////////////////////////////////////////////////////////

void TriangleMesh::onResourceLoaded( ResourcesManager& mgr )
{
   calculateBoundingVolume();
}

///////////////////////////////////////////////////////////////////////////////

void TriangleMesh::calculateBoundingVolume()
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

void TriangleMesh::calculateTangents()
{
   /**
    * I used the algorithm from this page: http://www.terathon.com/code/tangent.html.
    */

   uint vertexCount = m_vertices.size();
   Vector* tan1 = new Vector[vertexCount * 2];
   Vector* tan2 = tan1 + vertexCount;
   memset( tan1, 0, vertexCount * sizeof(Vector) * 2 );

   uint trianglesCount = m_faces.size();
   for ( uint i = 0; i < trianglesCount; ++i )
   {
      const Face& tri = m_faces[i];
      uint i1 = tri.idx[0];
      uint i2 = tri.idx[1];
      uint i3 = tri.idx[2];

      const TVector<3>& v1 = m_vertices[i1].m_coords;
      const TVector<3>& v2 = m_vertices[i2].m_coords;
      const TVector<3>& v3 = m_vertices[i3].m_coords;

      const TVector<2>& w1 = m_vertices[i1].m_textureCoords;
      const TVector<2>& w2 = m_vertices[i2].m_textureCoords;
      const TVector<2>& w3 = m_vertices[i3].m_textureCoords;

      float x1 = v2[0] - v1[0];
      float x2 = v3[0] - v1[0];
      float y1 = v2[1] - v1[1];
      float y2 = v3[1] - v1[1];
      float z1 = v2[2] - v1[2];
      float z2 = v3[2] - v1[2];

      float s1 = w2[0] - w1[0];
      float s2 = w3[0] - w1[0];
      float t1 = w2[1] - w1[1];
      float t2 = w3[1] - w1[1];

      float r = (s1 * t2 - s2 * t1);
      if ( r != 0.0f )
      { 
         r = 1.0f / r;

         Vector sdir( (t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r );
         Vector tdir( (s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r );

         tan1[i1].add( sdir );
         tan1[i2].add( sdir );
         tan1[i3].add( sdir );

         tan2[i1].add( tdir );
         tan2[i2].add( tdir );
         tan2[i3].add( tdir );
      }
   }

   Vector vertexNorm, vertexTangent, normTanCross;
   for ( uint i = 0; i < vertexCount; ++i )
   {
      vertexNorm.load( m_vertices[i].m_normal );
      const Vector& t = tan1[i];

      // Gram-Schmidt orthogonalize
      float normTan1Dot = vertexNorm.dot( t );
      vertexTangent.setMulAdd( vertexNorm, -normTan1Dot, t ).normalize();

      // Calculate handedness
      normTanCross.setCross( vertexNorm, t );
      float crossTan2Dot = normTanCross.dot( tan2[i] );
      float handedness =  ( crossTan2Dot < 0.0f ) ? -1.0f : 1.0f;
      vertexTangent.mul( 1.0f / handedness );

      // store the results
      vertexTangent.store( m_vertices[i].m_tangent );
   }

   delete[] tan1;
}

///////////////////////////////////////////////////////////////////////////////
