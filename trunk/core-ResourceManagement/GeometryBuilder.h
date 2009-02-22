#pragma once

#include <d3dx9.h>
#include <string>
#include <list>
#include <vector>
#include "BoundingVolume.h"
#include "Face.h"
#include "LitVertex.h"



///////////////////////////////////////////////////////////////////////////////

struct MeshData
{
   std::list<LitVertex> vertices;
   std::list<Face<USHORT> > indices;
   AABoundingBox boundingBox;
};

///////////////////////////////////////////////////////////////////////////////

class GeometryBuilder
{
public:
   MeshData* createPrism();

   MeshData* createCube();

private:
   template<typename vertex_type>
   AABoundingBox calculateBoundingBox(std::list<vertex_type>& vertices)
   {
      AABoundingBox boundingBox;
      boundingBox.min.x = boundingBox.min.y = boundingBox.min.z = 99999999.f;
      boundingBox.max.x = boundingBox.max.y = boundingBox.max.z = -99999999.f;

      for (typename std::list<vertex_type>::iterator it = vertices.begin(); 
            it != vertices.end(); ++it)
      {
         if (boundingBox.min.x > it->m_coords.x) boundingBox.min.x = it->m_coords.x;
         if (boundingBox.max.x < it->m_coords.x) boundingBox.max.x = it->m_coords.x;
         
         if (boundingBox.min.y > it->m_coords.y) boundingBox.min.y = it->m_coords.y;
         if (boundingBox.max.y < it->m_coords.y) boundingBox.max.y = it->m_coords.y;

         if (boundingBox.min.z > it->m_coords.z) boundingBox.min.z = it->m_coords.z;
         if (boundingBox.max.z < it->m_coords.z) boundingBox.max.z = it->m_coords.z;
      }

      return boundingBox;
   }
};

///////////////////////////////////////////////////////////////////////////////
