#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\MeshUtils.h"
#include "core\Math.h"


///////////////////////////////////////////////////////////////////////////////

TEST( VertexTangents, basic )
{
   std::vector< LitVertex > vertices;
   std::vector< Face > faces;

   //                               pos           norm        tangent     UV
   vertices.push_back( LitVertex(   1,  1,  0,    0, 0, -1,   0, 0, 0,    1, -1 ) );
   vertices.push_back( LitVertex(  -1, -1,  0,    0, 0, -1,   0, 0, 0,    0,  0 ) );
   vertices.push_back( LitVertex(  -1,  1,  0,    0, 0, -1,   0, 0, 0,    0, -1 ) );

   vertices.push_back( LitVertex(   1,  1,  0,    0, 0,  1,   0, 0, 0,    1, -1 ) );
   vertices.push_back( LitVertex(  -1, -1,  0,    0, 0,  1,   0, 0, 0,    0,  0 ) );
   vertices.push_back( LitVertex(  -1,  1,  0,    0, 0,  1,   0, 0, 0,    0, -1 ) );

   vertices.push_back( LitVertex(   0,  1,  1,    1, 0,  0,   0, 0, 0,    1, -1 ) );
   vertices.push_back( LitVertex(   0, -1, -1,    1, 0,  0,   0, 0, 0,    0,  0 ) );
   vertices.push_back( LitVertex(   0,  1, -1,    1, 0,  0,   0, 0, 0,    0, -1 ) );

   faces.push_back( Face( 0, 1, 2 ) );
   faces.push_back( Face( 3, 4, 5 ) );
   faces.push_back( Face( 6, 7, 8 ) );

   MeshUtils::calculateVertexTangents( faces, vertices );

   Vector tangentVec, binormalVec, normalVec, objSpaceNormal;
   Matrix tangentToObjectSpace;


   // face normal facing towards the camera ( texture will be rotated backwards with respect to tangent space )
   {
      tangentVec.load( vertices[0].m_tangent );
      normalVec.load( vertices[0].m_normal );
      binormalVec.setCross( normalVec, tangentVec );
      tangentToObjectSpace.setRows( tangentVec, binormalVec, normalVec );

      tangentToObjectSpace.transformNorm( Vector::OZ, objSpaceNormal );
      COMPARE_VEC( Vector::OZ_NEG, objSpaceNormal );

      tangentToObjectSpace.transformNorm( Vector::OX, objSpaceNormal );
      COMPARE_VEC( Vector::OX, objSpaceNormal );

      tangentToObjectSpace.transformNorm( Vector::OY, objSpaceNormal );
      COMPARE_VEC( Vector::OY_NEG, objSpaceNormal );

   }

   // face normal facing away from the camera ( tangent space normals match the object space normals )
   {
      tangentVec.load( vertices[3].m_tangent );
      normalVec.load( vertices[3].m_normal );
      binormalVec.setCross( normalVec, tangentVec );
      tangentToObjectSpace.setRows( tangentVec, binormalVec, normalVec );

      tangentToObjectSpace.transformNorm( Vector::OZ, objSpaceNormal );
      COMPARE_VEC( Vector::OZ, objSpaceNormal );
    
      tangentToObjectSpace.transformNorm( Vector::OX, objSpaceNormal );
      COMPARE_VEC( Vector::OX_NEG, objSpaceNormal );

      tangentToObjectSpace.transformNorm( Vector::OY, objSpaceNormal );
      COMPARE_VEC( Vector::OY_NEG, objSpaceNormal );
   }

   // mesh normal pointing to the right
   {
      tangentVec.load( vertices[6].m_tangent );
      normalVec.load( vertices[6].m_normal );
      binormalVec.setCross( normalVec, tangentVec );
      tangentToObjectSpace.setRows( tangentVec, binormalVec, normalVec );

      tangentToObjectSpace.transformNorm( Vector::OZ, objSpaceNormal );
      COMPARE_VEC( Vector::OX, objSpaceNormal );

      tangentToObjectSpace.transformNorm( Vector::OX, objSpaceNormal );
      COMPARE_VEC( Vector::OZ, objSpaceNormal );

      tangentToObjectSpace.transformNorm( Vector::OY, objSpaceNormal );
      COMPARE_VEC( Vector::OY_NEG, objSpaceNormal );
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST( VertexTangents, rotatedFace )
{
   std::vector< LitVertex > vertices;
   std::vector< Face > faces;

   //                               pos           norm        tangent     UV
   vertices.push_back( LitVertex(   1,  1,  0,    0, 0, -1,   0, 0, 0,    1, -1 ) );
   vertices.push_back( LitVertex(  -1, -1,  0,    0, 0, -1,   0, 0, 0,    0,  0 ) );
   vertices.push_back( LitVertex(  -1,  1,  0,    0, 0, -1,   0, 0, 0,    0, -1 ) );

   faces.push_back( Face( 0, 1, 2 ) );

   MeshUtils::calculateVertexTangents( faces, vertices );

   Vector tangentVec, normalVec;
   {
      tangentVec.load( vertices[0].m_tangent );
      normalVec.load( vertices[0].m_normal );
   }

   Quaternion rotQuat;
   rotQuat.setAxisAngle( Vector::OY, DEG2RAD( 180.0f ) );

   {
      Vector rotTangentVec, rotBinormalVec, rotNormalVec, objSpaceNormal;
      rotQuat.transform( tangentVec, rotTangentVec );
      rotQuat.transform( normalVec, rotNormalVec );

      rotBinormalVec.setCross( rotNormalVec, rotTangentVec );

      Matrix tangentToObjectSpace;
      tangentToObjectSpace.setRows( rotTangentVec, rotBinormalVec, rotNormalVec );

      tangentToObjectSpace.transformNorm( Vector::OZ, objSpaceNormal );
      // COMPARE_VEC( Vector::OZ_NEG, objSpaceNormal ); - non-rotated result
      COMPARE_VEC( Vector::OZ, objSpaceNormal );

      tangentToObjectSpace.transformNorm( Vector::OX, objSpaceNormal );
      // COMPARE_VEC( Vector::OX, objSpaceNormal ); - non-rotated result
      COMPARE_VEC( Vector::OX_NEG, objSpaceNormal );

      tangentToObjectSpace.transformNorm( Vector::OY, objSpaceNormal );
      // COMPARE_VEC( Vector::OY_NEG, objSpaceNormal ); - non-rotated result
      COMPARE_VEC( Vector::OY_NEG, objSpaceNormal );

   }
}

///////////////////////////////////////////////////////////////////////////////
