#pragma once

#include "core\BoundingVolume.h"
#include <d3dx9.h>
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

struct Triangle : public BoundingVolume
{
private:
   D3DXVECTOR3 v[3];
   D3DXVECTOR3 e[3];
   D3DXVECTOR3 en[3];

public:
   Triangle(const Triangle& rhs);

   /**
    * Make sure to specify the edges in the CLOCKWISE order,
    * otherwise the collision tests will fail
    */
   Triangle(const D3DXVECTOR3& pt1,
            const D3DXVECTOR3& pt2,
            const D3DXVECTOR3& pt3);

   const D3DXVECTOR3& vertex(unsigned int idx) const
   {
      ASSERT(idx <= 2, "Vertex index should be <= 2");
      return v[idx];
   }

   const D3DXVECTOR3& edge(unsigned int idx) const
   {
      ASSERT(idx <= 2, "Edge index should be <= 2");

      return e[idx];
   }

   const D3DXVECTOR3& edgeNormal(unsigned int idx) const
   {
      ASSERT(idx <= 2, "Edge normal index should be <= 2");

      return en[idx];
   }

   BoundingVolume* operator*(const D3DXMATRIX& mtx) const;

   bool testCollision(const PointVolume& point) const;
   bool testCollision(const AABoundingBox& rhs) const;
   bool testCollision(const BoundingSphere& rhs) const;
   bool testCollision(const Frustum& rhs) const;
   bool testCollision(const Ray& rhs) const;
   bool testCollision(const Triangle& rhs) const;
   bool testCollision(const BoundingVolume& rhs) const {return rhs.testCollision(*this);}
};

///////////////////////////////////////////////////////////////////////////////
