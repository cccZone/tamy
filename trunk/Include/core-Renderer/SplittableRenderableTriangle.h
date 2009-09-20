#pragma once

/// @file   core-Renderer\SplittableRenderableTriangle.h
/// @brief  a splittable triangle with renderable vertices

#include "core-Renderer\LitVertex.h"
#include "core\TriangleSplitter.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This structure describes triangles that have
 * an ability of being split using TriangleSplitter algorithm.
 */
template <>
struct SplittableTriangle<LitVertex>
{
   LitVertex vtx[3];

   SplittableTriangle();

   SplittableTriangle(const LitVertex& v1, 
                      const LitVertex& v2, 
                      const LitVertex& v3);

   const LitVertex& vertex(unsigned int vtxIdx) const;

   const D3DXVECTOR3& vertexPos(unsigned int vtxIdx) const;

   LitVertex splitEdge(float percentage, 
                       unsigned int startVtxIdx, 
                       unsigned int endVtxIdx) const;
};
typedef SplittableTriangle<LitVertex> MeshTriangle;

///////////////////////////////////////////////////////////////////////////////
