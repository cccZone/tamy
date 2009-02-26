#pragma once

#include <list>
#include <vector>
#include "LitVertex.h"
#include "Face.h"
#include <string>
#include "Color.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

struct MaterialDefinition
{
   std::string matName;

   Color ambient;
   Color diffuse;
   Color specular;
   Color emissive;
   float power;
   std::string texName;
};

///////////////////////////////////////////////////////////////////////////////

struct MeshDefinition
{
   std::string name;
   std::vector<MaterialDefinition> materials;
   std::list<LitVertex> vertices;
   std::list<Face<USHORT> > faces;
   D3DXMATRIX localMtx;

   std::list<MeshDefinition> children;
};

///////////////////////////////////////////////////////////////////////////////
