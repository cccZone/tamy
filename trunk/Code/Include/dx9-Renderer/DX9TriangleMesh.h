#pragma once

/// @file   dx9-Renderer\DX9TriangleMesh.h
/// @brief  dx9 implementation of a triangle mesh geometry

#include "core-Renderer\TriangleMesh.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class TriangleMesh;

///////////////////////////////////////////////////////////////////////////////

class DX9TriangleMesh : public TriangleMeshImpl
{
private:
   TriangleMesh& m_mesh;
   ID3DXMesh* m_dxMesh;

public:
   DX9TriangleMesh(TriangleMesh& mesh);
   ~DX9TriangleMesh();

   // -------------------------------------------------------------------------
   // TriangleMeshImpl implementation
   // -------------------------------------------------------------------------
   void render();

   void initialize(Renderer& renderer);
};

///////////////////////////////////////////////////////////////////////////////
