#pragma once

#include <d3dx9.h>
#include <iostream>
#include <string.h>
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

struct LitVertex
{
   // constructor for unskinned lit objects with no textures
   static LitVertex unskinnedNoTex(float x, float y, float z,
                                   float nx, float ny, float nz)
   {
      LitVertex vtx;
      
      vtx.m_coords.x = x;
      vtx.m_coords.y = y;
      vtx.m_coords.z = z;
      
      vtx.m_normal.x = nx;
      vtx.m_normal.y = ny;
      vtx.m_normal.z = nz;

      return vtx;
   }

   // constructor for unskinned lit objects with one texture
   static LitVertex unskinnedOneTex(float x, float y, float z,
                                    float nx, float ny, float nz,
                                    float u, float v)
   {
      LitVertex vtx;

      vtx.m_texCoords[0] = u;
      vtx.m_texCoords[1] = v;
      
      vtx.m_coords.x = x;
      vtx.m_coords.y = y;
      vtx.m_coords.z = z;
      
      vtx.m_normal.x = nx;
      vtx.m_normal.y = ny;
      vtx.m_normal.z = nz;

      return vtx;
   }

   // constructor for skinned lit objects with no textures
   static LitVertex skinnedNoTex(float x, float y, float z,
                                  float w0, float w1, float w2,
                                  float nx, float ny, float nz)
   {
      LitVertex vtx;
      
      vtx.m_coords.x = x;
      vtx.m_coords.y = y;
      vtx.m_coords.z = z;
      
      vtx.m_normal.x = nx;
      vtx.m_normal.y = ny;
      vtx.m_normal.z = nz;

      vtx.m_weight0 = w0;
      vtx.m_weight1 = w1;
      vtx.m_weight2 = w2;

      return vtx;
   }

   // constructor for unskinned lit objects with one texture
   static LitVertex skinnedOneTex(float x, float y, float z,
                                  float w0, float w1, float w2,
                                  float nx, float ny, float nz,
                                  float u, float v)
   {
      LitVertex vtx;
      
      vtx.m_texCoords[0] = u;
      vtx.m_texCoords[1] = v;
      
      vtx.m_coords.x = x;
      vtx.m_coords.y = y;
      vtx.m_coords.z = z;
      
      vtx.m_normal.x = nx;
      vtx.m_normal.y = ny;
      vtx.m_normal.z = nz;

      vtx.m_weight0 = w0;
      vtx.m_weight1 = w1;
      vtx.m_weight2 = w2;

      return vtx;
   }

   // -------------------------------------------------------------------------

   LitVertex()
      : m_coords(0, 0, 0), 
      m_weight0(0), m_weight1(0), m_weight2(0),
      m_normal(0, 0, 0)
   {
      m_texCoords[0] = 0;
      m_texCoords[1] = 0;
   }

   // -------------------------------------------------------------------------

   D3DXVECTOR3 m_coords;
   float m_weight0;
   float m_weight1;
   float m_weight2;
   D3DXVECTOR3 m_normal;
   float m_texCoords[2];

   static DWORD FVF;

   void serialize(Serializer& serializer)
   {
      serializer << m_coords;
      serializer << m_weight0;
      serializer << m_weight1;
      serializer << m_weight2;
      serializer << m_normal;
      serializer << m_texCoords[0];
      serializer << m_texCoords[1];
   }
};

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& stream, const LitVertex& vertex);

///////////////////////////////////////////////////////////////////////////////