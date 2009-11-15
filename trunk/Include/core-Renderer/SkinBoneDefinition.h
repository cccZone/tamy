#pragma once

/// @file   core-Renderer\SkinBoneDefinition.h
/// @brief  description of a skin bone

#include <string>
#include <vector>
#include <d3dx9.h>
#include "core\Serializable.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

typedef std::vector<std::string> BonesInfluenceDefinition;

///////////////////////////////////////////////////////////////////////////////

struct SkinBoneDefinition : public Serializable
{
   std::string name;
   // the bone offset matrix is unique for a bone, and not for a vertex that
   // references the bone in the skin
   D3DXMATRIX boneOffset;

   SkinBoneDefinition() 
   {
      D3DXMatrixIdentity(&boneOffset);
   }

   SkinBoneDefinition(const std::string& _name,
                      const D3DXMATRIX& _boneOffset)
         : name(_name), boneOffset(_boneOffset)
   {
   }

   // -------------------------------------------------------------------------
   // Serializable implementation
   // -------------------------------------------------------------------------
   void save(Serializer& serializer)
   {
      serializer.saveString(name);
      serializer.saveMatrix(boneOffset);
   }

   void load(Serializer& serializer)
   {
      serializer.loadString(name);
      serializer.loadMatrix(boneOffset);
   }
};

///////////////////////////////////////////////////////////////////////////////
