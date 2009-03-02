#pragma once

#include <list>
#include <vector>
#include "LitVertex.h"
#include "Face.h"
#include <string>
#include "Color.h"
#include <d3dx9.h>
#include "SkinBoneDefinition.h"


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
   bool isSkin;

   std::vector<MaterialDefinition> materials;
   std::list<LitVertex> vertices;
   std::list<Face<USHORT> > faces;

   // these two attributes have meaning only if the mesh is a skin
   std::vector<BonesInfluenceDefinition> bonesInfluencingAttribute;
   std::vector<SkinBoneDefinition> skinBones;

   D3DXMATRIX localMtx;

   std::list<MeshDefinition> children;

   MeshDefinition() : isSkin(false) {}
};

///////////////////////////////////////////////////////////////////////////////
