#include "DebugGeometryBuilder.h"
#include "core-Renderer/TriangleMesh.h"
#include "core-Renderer/LineSegments.h"


///////////////////////////////////////////////////////////////////////////////

TriangleMesh* DebugGeometryBuilder::createBox( const Vector& min, const Vector& max )
{
   std::vector<LitVertex> vertices;
   std::vector<Face> faces;
   vertices.resize( 8 );
   faces.resize( 12 );

   Vector tmpVec;
   {
      // bottom face vertices
      tmpVec.set( min.x, min.y, max.z ); tmpVec.store( vertices[0].m_coords );
      tmpVec.set( min.x, min.y, min.z ); tmpVec.store( vertices[1].m_coords );
      tmpVec.set( max.x, min.y, max.z ); tmpVec.store( vertices[2].m_coords );
      tmpVec.set( max.x, min.y, min.z ); tmpVec.store( vertices[3].m_coords );

      // top face vertices
      tmpVec.set( min.x, max.y, max.z ); tmpVec.store( vertices[4].m_coords );
      tmpVec.set( min.x, max.y, min.z ); tmpVec.store( vertices[5].m_coords );
      tmpVec.set( max.x, max.y, max.z ); tmpVec.store( vertices[6].m_coords );
      tmpVec.set( max.x, max.y, min.z ); tmpVec.store( vertices[7].m_coords );

      // add the lines:
      // bottom face
      faces[0].idx[0] = 0; faces[0].idx[1] = 1; faces[0].idx[2] = 2;
      faces[1].idx[0] = 1; faces[1].idx[1] = 3; faces[1].idx[2] = 2;

      // top face
      faces[2].idx[0] = 5; faces[2].idx[1] = 4; faces[2].idx[2] = 7;
      faces[3].idx[0] = 4; faces[3].idx[1] = 6; faces[3].idx[2] = 7;

      // right face
      faces[4].idx[0] = 1; faces[4].idx[1] = 5; faces[4].idx[2] = 3;
      faces[5].idx[0] = 5; faces[5].idx[1] = 7; faces[5].idx[2] = 3;

      // left face
      faces[6].idx[0] = 0; faces[6].idx[1] = 4; faces[6].idx[2] = 1;
      faces[7].idx[0] = 4; faces[7].idx[1] = 5; faces[7].idx[2] = 1;

      // front face
      faces[8].idx[0] = 2; faces[8].idx[1] = 6; faces[8].idx[2] = 0;
      faces[9].idx[0] = 6; faces[9].idx[1] = 4; faces[9].idx[2] = 0;

      // back face
      faces[10].idx[0] = 3; faces[10].idx[1] = 7; faces[10].idx[2] = 2;
      faces[11].idx[0] = 7; faces[11].idx[1] = 6; faces[11].idx[2] = 2;
   }

   TriangleMesh* mesh = new TriangleMesh( FilePath(), vertices, faces );
   return mesh;
}

///////////////////////////////////////////////////////////////////////////////

TriangleMesh* DebugGeometryBuilder::createArrow( float size, const Vector& start, const Vector& end )
{
   Vector dir;
   dir.setSub( end, start ).normalize();

   Vector perpVec1, perpVec2;
   VectorUtil::calculatePerpendicularVector( dir, perpVec1 );
   perpVec1.normalize();
   perpVec2.setCross( dir, perpVec1 );

   perpVec1.mul( size );
   perpVec2.mul( size );


   // the line
   std::vector<LitVertex> vertices;
   std::vector<Face> faces;
   addCuboid( size, start, end, vertices, faces );

   // the cone tip
   const float tipSizeMultiplier = 1.3f;
   Vector tipEnd;
   tipEnd.setMulAdd( dir, size * tipSizeMultiplier, end );
   addCone( size * tipSizeMultiplier, end, tipEnd, vertices, faces );
   TriangleMesh* mesh = new TriangleMesh( FilePath(), vertices, faces );
   return mesh;
}

///////////////////////////////////////////////////////////////////////////////

TriangleMesh* DebugGeometryBuilder::createBoxHeadedArrow( float size, const Vector& start, const Vector& end )
{
   Vector dir;
   dir.setSub( end, start ).normalize();

   Vector perpVec1, perpVec2;
   VectorUtil::calculatePerpendicularVector( dir, perpVec1 );
   perpVec1.normalize();
   perpVec2.setCross( dir, perpVec1 );

   perpVec1.mul( size );
   perpVec2.mul( size );


   // the line
   std::vector<LitVertex> vertices;
   std::vector<Face> faces;
   addCuboid( size, start, end, vertices, faces );
   
   // and the box-shaped tip
   const float tipSizeMultiplier = 6;
   Vector tipEnd;
   tipEnd.setMulAdd( dir, size * tipSizeMultiplier, end );
   addCuboid( tipSizeMultiplier * size, end, tipEnd, vertices, faces );
   
   TriangleMesh* mesh = new TriangleMesh( FilePath(), vertices, faces );
   return mesh;
}

///////////////////////////////////////////////////////////////////////////////

void DebugGeometryBuilder::addCone( float baseSize, const Vector& start, const Vector& end, std::vector< LitVertex >& outVertices, std::vector< Face >& outFaces )
{
   // calculate additional vectors needed for cuboid construction
   Vector dir;
   dir.setSub( end, start ).normalize();

   Vector perpVec1, perpVec2;
   VectorUtil::calculatePerpendicularVector( dir, perpVec1 );
   perpVec1.normalize();
   perpVec2.setCross( dir, perpVec1 );

   perpVec1.mul( baseSize );
   perpVec2.mul( baseSize );

   // calculate the vertices and outFaces
   uint firstVtxIdx = outVertices.size();
   uint firstFaceIdx = outFaces.size();

   for( uint i = 0; i < 5; ++i )
   {
      outVertices.push_back( LitVertex() );
   }
   for( uint i = 0; i < 6; ++i )
   {
      outFaces.push_back( Face() );
   }

   Vector tmpVec;
   {
      const float tipSizeMultiplier = 6;
      perpVec1.mul( tipSizeMultiplier );
      perpVec2.mul( tipSizeMultiplier );

      tmpVec.setAdd( start, perpVec1 ).sub( perpVec2 ); tmpVec.store( outVertices[firstVtxIdx + 0].m_coords );
      tmpVec.setSub( start, perpVec1 ).sub( perpVec2 ); tmpVec.store( outVertices[firstVtxIdx + 1].m_coords );
      tmpVec.setAdd( start, perpVec1 ).add( perpVec2 ); tmpVec.store( outVertices[firstVtxIdx + 2].m_coords );
      tmpVec.setSub( start, perpVec1 ).add( perpVec2 ); tmpVec.store( outVertices[firstVtxIdx + 3].m_coords );
      tmpVec.setMulAdd( dir, baseSize * tipSizeMultiplier * 2, end ); tmpVec.store( outVertices[firstVtxIdx + 4].m_coords );

      // cone bottom
      outFaces[firstFaceIdx + 0].idx[0] = firstVtxIdx + 0; outFaces[firstFaceIdx + 0].idx[1] = firstVtxIdx + 1; outFaces[firstFaceIdx + 0].idx[2] = firstVtxIdx + 2;
      outFaces[firstFaceIdx + 1].idx[0] = firstVtxIdx + 1; outFaces[firstFaceIdx + 1].idx[1] = firstVtxIdx + 3; outFaces[firstFaceIdx + 1].idx[2] = firstVtxIdx + 2;

      // cone top
      outFaces[firstFaceIdx + 2].idx[0] = firstVtxIdx + 0;   outFaces[firstFaceIdx + 2].idx[1] = firstVtxIdx + 4;   outFaces[firstFaceIdx + 2].idx[2] = firstVtxIdx + 1;
      outFaces[firstFaceIdx + 3].idx[0] = firstVtxIdx + 1;   outFaces[firstFaceIdx + 3].idx[1] = firstVtxIdx + 4;   outFaces[firstFaceIdx + 3].idx[2] = firstVtxIdx + 3;
      outFaces[firstFaceIdx + 4].idx[0] = firstVtxIdx + 3;   outFaces[firstFaceIdx + 4].idx[1] = firstVtxIdx + 4;   outFaces[firstFaceIdx + 4].idx[2] = firstVtxIdx + 2;
      outFaces[firstFaceIdx + 5].idx[0] = firstVtxIdx + 2;   outFaces[firstFaceIdx + 5].idx[1] = firstVtxIdx + 4;   outFaces[firstFaceIdx + 5].idx[2] = firstVtxIdx + 0;
   }

}

///////////////////////////////////////////////////////////////////////////////

void DebugGeometryBuilder::addCuboid( float size, const Vector& start, const Vector& end, std::vector< LitVertex >& outVertices, std::vector< Face >& outFaces )
{
   // calculate additional vectors needed for cuboid construction
   Vector dir;
   dir.setSub( end, start ).normalize();

   Vector perpVec1, perpVec2;
   VectorUtil::calculatePerpendicularVector( dir, perpVec1 );
   perpVec1.normalize();
   perpVec2.setCross( dir, perpVec1 );

   perpVec1.mul( size );
   perpVec2.mul( size );


   // draw the cuboid
   uint firstVtxIdx = outVertices.size();
   uint firstFaceIdx = outFaces.size();

   for( uint i = 0; i < 8; ++i )
   {
      outVertices.push_back( LitVertex() );
   }
   for( uint i = 0; i < 12; ++i )
   {
      outFaces.push_back( Face() );
   }
   Vector tmpVec;
   {
      // bottom face vertices
      tmpVec.setAdd( start, perpVec1 ).sub( perpVec2 ); tmpVec.store( outVertices[firstVtxIdx + 0].m_coords );
      tmpVec.setSub( start, perpVec1 ).sub( perpVec2 ); tmpVec.store( outVertices[firstVtxIdx + 1].m_coords );
      tmpVec.setAdd( start, perpVec1 ).add( perpVec2 ); tmpVec.store( outVertices[firstVtxIdx + 2].m_coords );
      tmpVec.setSub( start, perpVec1 ).add( perpVec2 ); tmpVec.store( outVertices[firstVtxIdx + 3].m_coords );

      // top face vertices
      tmpVec.setAdd( end, perpVec1 ).sub( perpVec2 ); tmpVec.store( outVertices[firstVtxIdx + 4].m_coords );
      tmpVec.setSub( end, perpVec1 ).sub( perpVec2 ); tmpVec.store( outVertices[firstVtxIdx + 5].m_coords );
      tmpVec.setAdd( end, perpVec1 ).add( perpVec2 ); tmpVec.store( outVertices[firstVtxIdx + 6].m_coords );
      tmpVec.setSub( end, perpVec1 ).add( perpVec2 ); tmpVec.store( outVertices[firstVtxIdx + 7].m_coords );

      // add the lines:
      // bottom face
      outFaces[firstFaceIdx + 0].idx[0] = firstVtxIdx + 0; outFaces[firstFaceIdx + 0].idx[1] = firstVtxIdx + 1; outFaces[firstFaceIdx + 0].idx[2] = firstVtxIdx + 2;
      outFaces[firstFaceIdx + 1].idx[0] = firstVtxIdx + 1; outFaces[firstFaceIdx + 1].idx[1] = firstVtxIdx + 3; outFaces[firstFaceIdx + 1].idx[2] = firstVtxIdx + 2;

      // top face
      outFaces[firstFaceIdx + 2].idx[0] = firstVtxIdx + 5; outFaces[firstFaceIdx + 2].idx[1] = firstVtxIdx + 4; outFaces[firstFaceIdx + 2].idx[2] = firstVtxIdx + 7;
      outFaces[firstFaceIdx + 3].idx[0] = firstVtxIdx + 4; outFaces[firstFaceIdx + 3].idx[1] = firstVtxIdx + 6; outFaces[firstFaceIdx + 3].idx[2] = firstVtxIdx + 7;

      // right face
      outFaces[firstFaceIdx + 4].idx[0] = firstVtxIdx + 1; outFaces[firstFaceIdx + 4].idx[1] = firstVtxIdx + 5; outFaces[firstFaceIdx + 4].idx[2] = firstVtxIdx + 3;
      outFaces[firstFaceIdx + 5].idx[0] = firstVtxIdx + 5; outFaces[firstFaceIdx + 5].idx[1] = firstVtxIdx + 7; outFaces[firstFaceIdx + 5].idx[2] = firstVtxIdx + 3;

      // left face
      outFaces[firstFaceIdx + 6].idx[0] = firstVtxIdx + 0; outFaces[firstFaceIdx + 6].idx[1] = firstVtxIdx + 4; outFaces[firstFaceIdx + 6].idx[2] = firstVtxIdx + 1;
      outFaces[firstFaceIdx + 7].idx[0] = firstVtxIdx + 4; outFaces[firstFaceIdx + 7].idx[1] = firstVtxIdx + 5; outFaces[firstFaceIdx + 7].idx[2] = firstVtxIdx + 1;

      // front face
      outFaces[firstFaceIdx + 8].idx[0] = firstVtxIdx + 2; outFaces[firstFaceIdx + 8].idx[1] = firstVtxIdx + 6; outFaces[firstFaceIdx + 8].idx[2] = firstVtxIdx + 0;
      outFaces[firstFaceIdx + 9].idx[0] = firstVtxIdx + 6; outFaces[firstFaceIdx + 9].idx[1] = firstVtxIdx + 4; outFaces[firstFaceIdx + 9].idx[2] = firstVtxIdx + 0;

      // back face
      outFaces[firstFaceIdx + 10].idx[0] = firstVtxIdx + 3; outFaces[firstFaceIdx + 10].idx[1] = firstVtxIdx + 7; outFaces[firstFaceIdx + 10].idx[2] = firstVtxIdx + 2;
      outFaces[firstFaceIdx + 11].idx[0] = firstVtxIdx + 7; outFaces[firstFaceIdx + 11].idx[1] = firstVtxIdx + 6; outFaces[firstFaceIdx + 11].idx[2] = firstVtxIdx + 2;
   }
}

///////////////////////////////////////////////////////////////////////////////

TriangleMesh* DebugGeometryBuilder::createTorus( float innerRadius, float outerRadius, const Matrix& transform, int segmentsCount, int segmentVerticesCount )
{
   const Vector& origin = transform.position();
   const Vector& mainAxis = transform.forwardVec();
   const Vector& sideAxis = transform.sideVec();
   Vector circumferenceAxis = transform.upVec();

   float circumferenceWidth = outerRadius - innerRadius;
   float radius = innerRadius + circumferenceWidth;

   // calculate torus vertices
   const uint verticesCount = segmentsCount * segmentVerticesCount;
   std::vector<LitVertex> vertices( verticesCount );
   {
      const float dMainAngle = DEG2RAD( 360.0f / (float)segmentsCount );
      const float dSegmentAngle = DEG2RAD( 360.0f / (float)segmentVerticesCount );
      float mainAngle = 0.0f;

      Quaternion mainRot, circumferenceRot;
      Vector vtxPos, posOnCircumference, radiusDisplacement, radiusVec, torusVtx;
      radiusVec.setMul( sideAxis, radius );
      uint vtxIdx = 0;
      for ( uint segmentIdx = 0; segmentIdx < segmentsCount; ++segmentIdx, mainAngle += dMainAngle )
      {
         mainRot.setAxisAngle( mainAxis, mainAngle );
         mainRot.transform( radiusVec, radiusDisplacement );

         float segmentAngle = 0.0f;
         for ( uint segVtxIdx = 0; segVtxIdx < segmentVerticesCount; ++segVtxIdx, segmentAngle += dSegmentAngle )
         {
            circumferenceRot.setAxisAngle( circumferenceAxis, segmentAngle );
            Quaternion rotQ;
            rotQ.setMul( circumferenceRot, mainRot );

            // first - create a point on the circumference of the toruses' segment
            vtxPos.setMul( sideAxis, circumferenceWidth );
            rotQ.transform( vtxPos, posOnCircumference );

            // and displace the circumference point so that it ends up in its final position on the toruses' circumference
            torusVtx.setAdd( radiusDisplacement, posOnCircumference );
            torusVtx.add( origin );

            // store the vertex
            torusVtx.store( vertices[vtxIdx].m_coords );
            ++vtxIdx;
         }
      }
   }

   // set torus indices
   const uint outFacesCount = verticesCount * 2;
   std::vector<Face> outFaces( outFacesCount );
   {
      uint faceIdx = 0;
      for ( uint segmentIdx = 0; segmentIdx < segmentsCount; ++segmentIdx )
      {
         uint currSegmentFirstVtx = segmentIdx * segmentVerticesCount;
         uint nextSegmentFirstVtx = ( currSegmentFirstVtx + segmentVerticesCount ) % verticesCount;

         for ( uint segVtxIdx = 0; segVtxIdx < segmentVerticesCount; ++segVtxIdx )
         {
            uint skipOffset = 0;
            if ( segVtxIdx == 0 )
            {
               skipOffset = segmentVerticesCount;
            }

            outFaces[faceIdx].idx[0] = currSegmentFirstVtx + segVtxIdx - 1 + skipOffset;
            outFaces[faceIdx].idx[1] = currSegmentFirstVtx + segVtxIdx;
            outFaces[faceIdx].idx[2] = nextSegmentFirstVtx + segVtxIdx - 1 + skipOffset;
            ++faceIdx;

            outFaces[faceIdx].idx[0] = currSegmentFirstVtx + segVtxIdx;
            outFaces[faceIdx].idx[1] = nextSegmentFirstVtx + segVtxIdx;
            outFaces[faceIdx].idx[2] = nextSegmentFirstVtx + segVtxIdx - 1 + skipOffset;
            ++faceIdx;
         }
      }
   }

   TriangleMesh* mesh = new TriangleMesh( FilePath(), vertices, outFaces );
   return mesh;
}

///////////////////////////////////////////////////////////////////////////////

LineSegments* DebugGeometryBuilder::createGrid( float size, float spacing, const Color& color )
{
   LineSegments* mesh = new LineSegments();

   const float dim = spacing <= 0 ? 0 : ( size / spacing );
   float varPos;
   for ( float i = -dim; i <= dim; ++i )
   {
      varPos = i * spacing;
      mesh->add( LineSegment( Vector( -size, 0, varPos ), Vector( size, 0, varPos ), color ) );
      mesh->add( LineSegment( Vector( varPos, 0, -size ), Vector( varPos, 0, size ), color ) );
   }

   return mesh;
}

///////////////////////////////////////////////////////////////////////////////
