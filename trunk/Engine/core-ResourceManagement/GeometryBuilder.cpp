#include "core-ResourceManagement\GeometryBuilder.h"


///////////////////////////////////////////////////////////////////////////////

MeshData* GeometryBuilder::createPrism()
{
   MeshData* result = new MeshData;
   
   // prysm base
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2, -2, -2, 0, 0, -1));
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2,  2, -2, 0, 0, -1));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2,  2, -2, 0, 0, -1));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2, -2, -2, 0, 0, -1));
   result->indices.push_back(Face(0, 1, 2, 0));
   result->indices.push_back(Face(0, 2, 3, 0));

   // top side
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2,  2, -2, 0, 0.707f, 0.707f));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 0,  0,  2, 0, 0.707f, 0.707f));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2,  2, -2, 0, 0.707f, 0.707f));
   result->indices.push_back(Face(4, 5, 6, 0));

   // right side
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2,  2, -2, 0.707f, 0, 0.707f));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 0,  0,  2, 0.707f, 0, 0.707f));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2, -2, -2, 0.707f, 0, 0.707f));
   result->indices.push_back(Face(7, 8, 9, 0));

   // bottom side
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2, -2, -2, 0, -0.707f, 0.707f));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 0,  0,  2, 0, -0.707f, 0.707f));
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2, -2, -2, 0, -0.707f, 0.707f));
   result->indices.push_back(Face(10, 11, 12, 0));

   // left side
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2, -2, -2, 0.707f, 0, -0.707f));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 0,  0,  2, 0.707f, 0, -0.707f));
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2,  2, -2, 0.707f, 0, -0.707f));
   result->indices.push_back(Face(13, 14, 15, 0));

   return result;
}

///////////////////////////////////////////////////////////////////////////////

MeshData* GeometryBuilder::createCube()
{
   MeshData* result = new MeshData;

   // back face
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2, -2, -2, 0, 0, -1));
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2,  2, -2, 0, 0, -1));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2,  2, -2, 0, 0, -1));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2, -2, -2, 0, 0, -1));
   result->indices.push_back(Face(0, 1, 2, 0));
   result->indices.push_back(Face(0, 2, 3, 0));

   // right face
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2, -2, -2, 1, 0, 0));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2,  2, -2, 1, 0, 0));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2,  2,  2, 1, 0, 0));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2, -2,  2, 1, 0, 0));
   result->indices.push_back(Face(4, 5, 6, 0));
   result->indices.push_back(Face(4, 6, 7, 0));

   // front face
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2, -2,  2, 0, 0, 1));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2,  2,  2, 0, 0, 1));
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2,  2,  2, 0, 0, 1));
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2, -2,  2, 0, 0, 1));
   result->indices.push_back(Face(8, 9, 10, 0));
   result->indices.push_back(Face(8, 10, 11, 0));
   
   // left face
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2, -2,  2, -1, 0, 0));
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2,  2,  2, -1, 0, 0));
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2,  2, -2, -1, 0, 0));
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2, -2, -2, -1, 0, 0));
   result->indices.push_back(Face(12, 13, 14, 0));
   result->indices.push_back(Face(12, 14, 15, 0));

   // top face
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2, 2, -2, 0, 1, 0));
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2, 2,  2, 0, 1, 0));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2, 2,  2, 0, 1, 0));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2, 2, -2, 0, 1, 0));
   result->indices.push_back(Face(16, 17, 18, 0));
   result->indices.push_back(Face(16, 18, 19, 0));

   // bottom face
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2, -2,  2, 0, -1, 0));
   result->vertices.push_back(LitVertex::unskinnedNoTex(-2, -2, -2, 0, -1, 0));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2, -2, -2, 0, -1, 0));
   result->vertices.push_back(LitVertex::unskinnedNoTex( 2, -2,  2, 0, -1, 0));
   result->indices.push_back(Face(20, 21, 22, 0));
   result->indices.push_back(Face(20, 22, 23, 0));

   return result;
}

///////////////////////////////////////////////////////////////////////////////
