/// @file   ml-IWF\MeshDefinition.h
/// @brief  definitions of mesh-related entities loaded by resource 
///         management system
#pragma once

#include <list>
#include <vector>
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\Face.h"
#include <string>
#include "core\Color.h"
#include "core\Math.h"

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

   MaterialDefinition()
      : power(1)
   {}
};

///////////////////////////////////////////////////////////////////////////////

struct MeshDefinition
{
   // -------------------------------------------------------------------------

   std::string name;

   MaterialDefinition material;
   std::vector<LitVertex> vertices;
   std::vector<Face > faces;

   Matrix localMtx;

   // -------------------------------------------------------------------------

   MeshDefinition()
   {
      localMtx = Matrix::IDENTITY;
   }

   MeshDefinition(const MeshDefinition& rhs)
   : name(rhs.name)
   , material(rhs.material)
   , vertices(rhs.vertices)
   , faces(rhs.faces)
   , localMtx(rhs.localMtx)
   {
   }

   MeshDefinition& operator=(const MeshDefinition& rhs)
   {
      name = rhs.name;
      material = rhs.material;
      vertices = rhs.vertices;
      faces = rhs.faces;
      localMtx = rhs.localMtx;

      return *this;
   }
};

///////////////////////////////////////////////////////////////////////////////
