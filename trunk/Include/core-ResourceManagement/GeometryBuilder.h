#pragma once

#include <d3dx9.h>
#include <string>
#include <list>
#include <vector>
#include "core-Renderer\Face.h"
#include "core-Renderer\LitVertex.h"



///////////////////////////////////////////////////////////////////////////////

struct MeshData
{
   std::list<LitVertex> vertices;
   std::list<Face > indices;
};

///////////////////////////////////////////////////////////////////////////////

class GeometryBuilder
{
public:
   MeshData* createPrism();

   MeshData* createCube();
};

///////////////////////////////////////////////////////////////////////////////
