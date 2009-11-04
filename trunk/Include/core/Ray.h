#pragma once

#include "core\BoundingVolume.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * the structure describes a ray
 */
struct Ray : public BoundingVolume
{
   D3DXVECTOR3 origin;
   D3DXVECTOR3 direction;

   Ray() {}

   Ray(const D3DXVECTOR3& _origin, const D3DXVECTOR3& _direction)
      : origin(_origin), direction(_direction)
   {}

   BoundingVolume* operator*(const D3DXMATRIX& mtx) const;

   PlaneClassification classifyAgainsPlane(const D3DXPLANE& plane) const;
   float distanceToPlane(const D3DXPLANE& plane) const;
   bool testCollision(const PointVolume& point) const;
   bool testCollision(const AABoundingBox& rhs) const;
   bool testCollision(const BoundingSphere& rhs) const;
   bool testCollision(const Frustum& rhs) const;
   bool testCollision(const Ray& rhs) const;
   bool testCollision(const Triangle& rhs) const;
   bool testCollision(const BoundingVolume& rhs) const {return rhs.testCollision(*this);}

};

///////////////////////////////////////////////////////////////////////////////
