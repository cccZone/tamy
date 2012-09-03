/// @file   ml-Blender/TamyMesh.h
/// @brief  Tamy mesh export-related proxies
#pragma once


///////////////////////////////////////////////////////////////////////////////

struct LitVertex;
struct Face;

///////////////////////////////////////////////////////////////////////////////

struct TamyMesh
{
   const char*    name;

   LitVertex*     verticesList;
   Face*          facesList;
   int*           materialsList;

   int            facesCount;
   int            verticesCount;
   int            materialsCount;
};

///////////////////////////////////////////////////////////////////////////////
