#pragma once

#include "core\BoundingVolume.h"
#include <d3dx9.h>


enum PlanesEnum
{
   FP_BOTTOM,
   FP_TOP,
   FP_LEFT,
   FP_RIGHT,
   FP_FAR,
   FP_NEAR,
};
///////////////////////////////////////////////////////////////////////////////

struct Frustum : public BoundingVolume
{
   D3DXPLANE planes[6];

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
};

///////////////////////////////////////////////////////////////////////////////
