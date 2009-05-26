#pragma once

#include <d3dx9.h>
#include <string>
#include <list>
#include <vector>
#include "core-ResourceManagement\Face.h"
#include "core-ResourceManagement\LitVertex.h"



///////////////////////////////////////////////////////////////////////////////

struct MeshData
{
   std::list<LitVertex> vertices;
   std::list<Face<USHORT> > indices;
};

///////////////////////////////////////////////////////////////////////////////

class GeometryBuilder
{
public:
   MeshData* createPrism();

   MeshData* createCube();
};

///////////////////////////////////////////////////////////////////////////////
