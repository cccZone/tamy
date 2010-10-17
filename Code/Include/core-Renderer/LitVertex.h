#pragma once

#include <d3dx9.h>
#include <iostream>
#include <string.h>
#include "core\Serializer.h"


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

   void serialize( Serializer& serializer )
   {
      serializer << m_coords;
      serializer << m_normal;
      serializer << m_textureCoords;
   }
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
   
   void serialize( Serializer& serializer )
   {
      serializer << m_weights;
      serializer << m_indices;
   }
};

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<( std::ostream& stream, const LitVertex& vertex );
std::ostream& operator<<( std::ostream& stream, const VertexWeight& vertex );

///////////////////////////////////////////////////////////////////////////////