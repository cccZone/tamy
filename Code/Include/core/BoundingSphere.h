#pragma once

#include "core\BoundingVolume.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * The structure describes a sphere bounding an object
 */
struct BoundingSphere : public BoundingVolume
{
   D3DXVECTOR3 origin;
   float radius;

   BoundingSphere();
   BoundingSphere(const D3DXVECTOR3& origin, float radius);

   BoundingVolume* operator*(const D3DXMATRIX& mtx) const;
   void operator*=(const D3DXMATRIX& mtx);

   float distanceToPlane(const D3DXPLANE& plane) const;
   bool testCollision(const PointVolume& point) const;
   bool testCollision(const AABoundingBox& rhs) const;
   bool testCollision(const BoundingSphere& rhs) const;
   bool testCollision(const Frustum& rhs) const;
   bool testCollision(const Ray& rhs) const;
   bool testCollision(const Triangle& rhs) const;
   bool testCollision(const BoundingVolume& rhs) const {return rhs.testCollision(*this);}

protected:
   bool hasVolume() const;
};

///////////////////////////////////////////////////////////////////////////////