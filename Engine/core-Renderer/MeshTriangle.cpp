#include "core-Renderer\MeshTriangle.h"


///////////////////////////////////////////////////////////////////////////////

MeshTriangle::MeshTriangle()
{}

///////////////////////////////////////////////////////////////////////////////

MeshTriangle::MeshTriangle(const LitVertex& v1, 
                           const LitVertex& v2, 
                           const LitVertex& v3)
{
   vtx[0] = v1;
   vtx[1] = v2;
   vtx[2] = v3;
}

///////////////////////////////////////////////////////////////////////////////

MeshTriangle::~MeshTriangle()
{
}

///////////////////////////////////////////////////////////////////////////////

const LitVertex& MeshTriangle::vertex(unsigned int vtxIdx) const 
{
   return vtx[vtxIdx];
}

///////////////////////////////////////////////////////////////////////////////

const D3DXVECTOR3& MeshTriangle::vertexPos(unsigned int vtxIdx) const 
{
   return vtx[vtxIdx].m_coords;
}

///////////////////////////////////////////////////////////////////////////////

LitVertex MeshTriangle::splitEdge(float percentage, 
                                  unsigned int startVtxIdx, 
                                  unsigned int endVtxIdx) const
{
   LitVertex newVertex;
   const LitVertex& startVtx = vtx[startVtxIdx];
   const LitVertex& endVtx = vtx[endVtxIdx];

   newVertex.m_coords = startVtx.m_coords + (endVtx.m_coords - startVtx.m_coords) * percentage;
   newVertex.m_normal = startVtx.m_normal + (endVtx.m_normal - startVtx.m_normal) * percentage;
   newVertex.m_texCoords[0] = startVtx.m_texCoords[0] + (endVtx.m_texCoords[0] - startVtx.m_texCoords[0]) * percentage;
   newVertex.m_texCoords[1] = startVtx.m_texCoords[1] + (endVtx.m_texCoords[1] - startVtx.m_texCoords[1]) * percentage;

   // splitting weights doesn't matter in this case since we're not gonna
   // be splitting skins - however if that turns out necessary at some point - add it

   return newVertex;
}

///////////////////////////////////////////////////////////////////////////////
