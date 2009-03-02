#pragma once

#include <string>
#include <vector>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

typedef std::vector<std::string> BonesInfluenceDefinition;

///////////////////////////////////////////////////////////////////////////////

struct SkinBoneDefinition
{
   std::string name;
   D3DXMATRIX boneOffset;

   SkinBoneDefinition() {}

   SkinBoneDefinition(const std::string& _name,
                      const D3DXMATRIX& _boneOffset)
         : name(_name), boneOffset(_boneOffset)
   {
   }
};

///////////////////////////////////////////////////////////////////////////////
