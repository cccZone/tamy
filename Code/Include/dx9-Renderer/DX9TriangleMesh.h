#pragma once

/// @file   dx9-Renderer\DX9TriangleMesh.h
/// @brief  dx9 implementation of a triangle mesh geometry

#include "core-Renderer\TriangleMesh.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;
class TriangleMesh;

///////////////////////////////////////////////////////////////////////////////

class DX9TriangleMesh
{
private:
   const TriangleMesh&           m_mesh;
   const DX9Renderer&            m_renderer;
   IDirect3DDevice9*             m_d3Device;
   IDirect3DVertexBuffer9*       m_vb;
   IDirect3DIndexBuffer9*        m_ib;
   unsigned int                  m_verticesCount;
   unsigned int                  m_facesCount;

public:
   DX9TriangleMesh( const DX9Renderer& renderer, const TriangleMesh& mesh );
   ~DX9TriangleMesh();

   void render();

private:
   void initialize();
};

///////////////////////////////////////////////////////////////////////////////
