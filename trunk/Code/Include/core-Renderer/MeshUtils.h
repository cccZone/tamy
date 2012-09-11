/// @file   core-Renderer/MeshUtils.h
/// @brief  a set of triangle mesh related utilities
#pragma once

#include <vector>
#include "core-Renderer/Face.h"
#include "core-Renderer/LitVertex.h"

///////////////////////////////////////////////////////////////////////////////

class MeshUtils
{
public:
   /**
    * Calculates texture-space tangent vectors. 
    *
    * @param faces
    * @param inOutVertices
    */
   static void calculateVertexTangents( const std::vector< Face >& faces, std::vector< LitVertex >& inOutVertices );
};

///////////////////////////////////////////////////////////////////////////////
