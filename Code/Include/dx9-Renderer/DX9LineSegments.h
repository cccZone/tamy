#pragma once

/// @file   dx9-Renderer\DX9LineSegments.h
/// @brief  dx9 implementation of line segments geometry

#include "core-Renderer\LineSegments.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

class DX9LineSegments : public LineSegmentsImpl
{
private:
   struct LineVertex
   {
      D3DXVECTOR3 pos;
      D3DXVECTOR4 color;

      LineVertex( const D3DXVECTOR3& _pos, const D3DXVECTOR4& _color ) 
         : pos( _pos )
         , color( _color )
      {}
   };

private:
   static D3DVERTEXELEMENT9 s_vtxDecl[];

   LineSegments& m_lines;

   IDirect3DDevice9* m_d3Device;
   DX9Renderer* m_renderer;
   LPDIRECT3DVERTEXDECLARATION9 m_vertexDecl;
   IDirect3DVertexBuffer9* m_vb;
   UINT m_segsCount;

public:
   DX9LineSegments(LineSegments& lines);
   ~DX9LineSegments();

   // -------------------------------------------------------------------------
   // LineSegmentsImpl implementation
   // -------------------------------------------------------------------------
   void render();

   void update(const std::vector<LineSegment>& segments);

   void initialize(Renderer& renderer);

private:
   void resetVB();
};

///////////////////////////////////////////////////////////////////////////////


