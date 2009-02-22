#include "GeometryBuilder.h"


///////////////////////////////////////////////////////////////////////////////

MeshData* GeometryBuilder::createPrism()
{
   MeshData* result = new MeshData;
   
   // prysm base
   result->vertices.push_back(LitVertex(-2, -2, -2, 0, 0, -1, 0, 0));
   result->vertices.push_back(LitVertex(-2,  2, -2, 0, 0, -1, 0, 0));
   result->vertices.push_back(LitVertex( 2,  2, -2, 0, 0, -1, 0, 0));
   result->vertices.push_back(LitVertex( 2, -2, -2, 0, 0, -1, 0, 0));
   result->indices.push_back(Face<USHORT>(0, 1, 2, 0));
   result->indices.push_back(Face<USHORT>(0, 2, 3, 0));

   // top side
   result->vertices.push_back(LitVertex(-2,  2, -2, 0, 0.707f, 0.707f, 0, 0));
   result->vertices.push_back(LitVertex( 0,  0,  2, 0, 0.707f, 0.707f, 0, 0));
   result->vertices.push_back(LitVertex( 2,  2, -2, 0, 0.707f, 0.707f, 0, 0));
   result->indices.push_back(Face<USHORT>(4, 5, 6, 0));

   // right side
   result->vertices.push_back(LitVertex( 2,  2, -2, 0.707f, 0, 0.707f, 0, 0));
   result->vertices.push_back(LitVertex( 0,  0,  2, 0.707f, 0, 0.707f, 0, 0));
   result->vertices.push_back(LitVertex( 2, -2, -2, 0.707f, 0, 0.707f, 0, 0));
   result->indices.push_back(Face<USHORT>(7, 8, 9, 0));

   // bottom side
   result->vertices.push_back(LitVertex( 2, -2, -2, 0, -0.707f, 0.707f, 0, 0));
   result->vertices.push_back(LitVertex( 0,  0,  2, 0, -0.707f, 0.707f, 0, 0));
   result->vertices.push_back(LitVertex(-2, -2, -2, 0, -0.707f, 0.707f, 0, 0));
   result->indices.push_back(Face<USHORT>(10, 11, 12, 0));

   // left side
   result->vertices.push_back(LitVertex(-2, -2, -2, 0.707f, 0, -0.707f, 0, 0));
   result->vertices.push_back(LitVertex( 0,  0,  2, 0.707f, 0, -0.707f, 0, 0));
   result->vertices.push_back(LitVertex(-2,  2, -2, 0.707f, 0, -0.707f, 0, 0));
   result->indices.push_back(Face<USHORT>(13, 14, 15, 0));

   result->boundingBox = AABoundingBox(D3DXVECTOR3(-2, -2, -2), D3DXVECTOR3(2, 2, 2));

   return result;
}

///////////////////////////////////////////////////////////////////////////////

MeshData* GeometryBuilder::createCube()
{
   MeshData* result = new MeshData;

   // back face
   result->vertices.push_back(LitVertex(-2, -2, -2, 0, 0, -1, 0, 0));
   result->vertices.push_back(LitVertex(-2,  2, -2, 0, 0, -1, 0, 0));
   result->vertices.push_back(LitVertex( 2,  2, -2, 0, 0, -1, 0, 0));
   result->vertices.push_back(LitVertex( 2, -2, -2, 0, 0, -1, 0, 0));
   result->indices.push_back(Face<USHORT>(0, 1, 2, 0));
   result->indices.push_back(Face<USHORT>(0, 2, 3, 0));

   // right face
   result->vertices.push_back(LitVertex( 2, -2, -2, 1, 0, 0, 0, 0));
   result->vertices.push_back(LitVertex( 2,  2, -2, 1, 0, 0, 0, 0));
   result->vertices.push_back(LitVertex( 2,  2,  2, 1, 0, 0, 0, 0));
   result->vertices.push_back(LitVertex( 2, -2,  2, 1, 0, 0, 0, 0));
   result->indices.push_back(Face<USHORT>(4, 5, 6, 0));
   result->indices.push_back(Face<USHORT>(4, 6, 7, 0));

   // front face
   result->vertices.push_back(LitVertex( 2, -2,  2, 0, 0, 1, 0, 0));
   result->vertices.push_back(LitVertex( 2,  2,  2, 0, 0, 1, 0, 0));
   result->vertices.push_back(LitVertex(-2,  2,  2, 0, 0, 1, 0, 0));
   result->vertices.push_back(LitVertex(-2, -2,  2, 0, 0, 1, 0, 0));
   result->indices.push_back(Face<USHORT>(8, 9, 10, 0));
   result->indices.push_back(Face<USHORT>(8, 10, 11, 0));
   
   // left face
   result->vertices.push_back(LitVertex(-2, -2,  2, -1, 0, 0, 0, 0));
   result->vertices.push_back(LitVertex(-2,  2,  2, -1, 0, 0, 0, 0));
   result->vertices.push_back(LitVertex(-2,  2, -2, -1, 0, 0, 0, 0));
   result->vertices.push_back(LitVertex(-2, -2, -2, -1, 0, 0, 0, 0));
   result->indices.push_back(Face<USHORT>(12, 13, 14, 0));
   result->indices.push_back(Face<USHORT>(12, 14, 15, 0));

   // top face
   result->vertices.push_back(LitVertex(-2, 2, -2, 0, 1, 0, 0, 0));
   result->vertices.push_back(LitVertex(-2, 2,  2, 0, 1, 0, 0, 0));
   result->vertices.push_back(LitVertex( 2, 2,  2, 0, 1, 0, 0, 0));
   result->vertices.push_back(LitVertex( 2, 2, -2, 0, 1, 0, 0, 0));
   result->indices.push_back(Face<USHORT>(16, 17, 18, 0));
   result->indices.push_back(Face<USHORT>(16, 18, 19, 0));

   // bottom face
   result->vertices.push_back(LitVertex(-2, -2,  2, 0, -1, 0, 0, 0));
   result->vertices.push_back(LitVertex(-2, -2, -2, 0, -1, 0, 0, 0));
   result->vertices.push_back(LitVertex( 2, -2, -2, 0, -1, 0, 0, 0));
   result->vertices.push_back(LitVertex( 2, -2,  2, 0, -1, 0, 0, 0));
   result->indices.push_back(Face<USHORT>(20, 21, 22, 0));
   result->indices.push_back(Face<USHORT>(20, 22, 23, 0));

   result->boundingBox = AABoundingBox(D3DXVECTOR3(-2, -2, -2), D3DXVECTOR3(2, 2, 2));

   return result;
}

///////////////////////////////////////////////////////////////////////////////
