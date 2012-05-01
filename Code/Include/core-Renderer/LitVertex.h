#pragma once

#include <d3dx9.h>
#include <iostream>
#include <string.h>


///////////////////////////////////////////////////////////////////////////////

class InStream;
class OutStream;

///////////////////////////////////////////////////////////////////////////////

struct LitVertex
{
   D3DXVECTOR3 m_coords;
   D3DXVECTOR3 m_normal;
   D3DXVECTOR2 m_textureCoords;

   static DWORD FVF;

   /**
    * Constructor for a simple mesh vertex.
    */
   LitVertex( float x, float y, float z, float nx, float ny, float nz, float u, float v )
      : m_coords( x, y, z )
      , m_normal( nx, ny, nz )
      , m_textureCoords( u, v )
   {
   }

   LitVertex()
      : m_coords( 0, 0, 0 )
      , m_normal( 0, 0, 0 )
      , m_textureCoords( 0, 0 )
   {
   }

   friend OutStream& operator<<( OutStream& stream, const LitVertex& vtx );
   friend InStream& operator>>( InStream& stream, LitVertex& vtx );
};

///////////////////////////////////////////////////////////////////////////////

struct VertexWeight
{
   D3DXVECTOR4          m_weights;
   D3DXVECTOR4          m_indices;

   static DWORD FVF;

   VertexWeight()
      : m_weights( 0, 0, 0, 0 )
      , m_indices( -1, -1, -1, -1 )
   {
   }
   
   friend OutStream& operator<<( OutStream& stream, const VertexWeight& weight );
   friend InStream& operator>>( InStream& stream, VertexWeight& weight );
};

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<( std::ostream& stream, const LitVertex& vertex );
std::ostream& operator<<( std::ostream& stream, const VertexWeight& vertex );

///////////////////////////////////////////////////////////////////////////////