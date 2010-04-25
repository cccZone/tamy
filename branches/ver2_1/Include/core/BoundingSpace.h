#pragma once

#include "core\BoundingVolume.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Space is infinte - thus is this volume. It should be used
 * to bound things that should be considered ubiquitous,
 * like directional and ambient lighting
 */
struct BoundingSpace : public BoundingVolume
{
   float distanceToPlane(const D3DXPLANE& plane) const {return 0;}

   bool testCollision(const PointVolume& point) const {return true;}

   bool testCollision(const AABoundingBox& rhs) const {return true;}

   bool testCollision(const BoundingSphere& rhs) const {return true;}

   bool testCollision(const Frustum& rhs) const {return true;}

   bool testCollision(const Ray& rhs) const {return true;}

   bool testCollision(const Triangle& rhs) const {return true;}

   bool testCollision(const BoundingVolume& rhs) const {return true;}

   BoundingVolume* operator*(const D3DXMATRIX& mtx) const {return new BoundingSpace();}

protected:
   bool hasVolume() const {return true;}
};

///////////////////////////////////////////////////////////////////////////////
