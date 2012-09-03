/// @file   ml-Blender/TamyScene.h
/// @brief  Tamy scene export-related proxies

#include "ml-Blender/TamyMesh.h"
#include "core/Matrix.h"



///////////////////////////////////////////////////////////////////////////////

struct TamyEntity
{
   const char*          name;

   TamyMesh*            meshesList;
   Matrix               localMatrix;

   int                  meshesCount;
};

///////////////////////////////////////////////////////////////////////////////

struct TamyScene
{
   const char*    name;

   TamyEntity*    entities;

   int            entitiesCount;
};

///////////////////////////////////////////////////////////////////////////////
