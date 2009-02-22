#pragma once

#include <d3dx9.h>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////

struct LitVertex
{
   LitVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) 
      : m_coords(x, y, z), m_normal(nx, ny, nz), m_u(u), m_v(v) {}

   D3DXVECTOR3 m_coords;
   D3DXVECTOR3 m_normal;
   float m_u, m_v;

   static DWORD FVF;
};

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& stream, const D3DXVECTOR3& vec);

///////////////////////////////////////////////////////////////////////////////