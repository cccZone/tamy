#pragma once

#include <d3dx9.h>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////

struct LitVertex
{
   LitVertex(float x, float y, float z,
             float w0, float w1, float w2,
             float nx, float ny, float nz, 
             float u, float v) 
      : m_coords(x, y, z), 
      m_weight0(w0), m_weight1(w1), m_weight2(w2),
      m_normal(nx, ny, nz), 
      m_u(u), m_v(v) {}

   D3DXVECTOR3 m_coords;
   float m_weight0;
   float m_weight1;
   float m_weight2;
   D3DXVECTOR3 m_normal;
   float m_u, m_v;

   static DWORD FVF;
};

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& stream, const D3DXVECTOR3& vec);

///////////////////////////////////////////////////////////////////////////////