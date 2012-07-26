#pragma once

/// @file   dx9-Renderer\DX9LineSegments.h
/// @brief  dx9 implementation of line segments geometry

#include "core-Renderer\LineSegments.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

class DX9LineSegments
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
   const DX9Renderer&               m_renderer;
   IDirect3DDevice9&                m_d3Device;
   const LineSegments&              m_lines;

   IDirect3DVertexBuffer9*          m_vb;
   UINT                             m_segsCount;

public:
   /**
    * Constructor.
    *
    * @param renderer
    * @param lines
    */
   DX9LineSegments( const DX9Renderer& renderer, const LineSegments& lines );
   ~DX9LineSegments();

   void render();
   void onLostDevice();
   void onResetDevice();
 
private:
   void create();
   void destroy();
};

///////////////////////////////////////////////////////////////////////////////


