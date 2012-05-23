#pragma once

#include "core\BoundingVolume.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Space is infinite - thus is this volume. It should be used
 * to bound things that should be considered ubiquitous,
 * like directional and ambient lighting
 */
struct BoundingSpace : public BoundingVolume
{
   // -------------------------------------------------------------------------
   // BoundingVolume implementation
   // -------------------------------------------------------------------------
   BoundingVolume* clone() const { return new BoundingSpace(); }
   void transform( const Matrix& mtx, BoundingVolume& transformedVolume ) const {}
   float distanceToPlane( const Plane& plane ) const { return 0; }
   bool testCollision( const PointVolume& point ) const { return true; }
   bool testCollision( const AABoundingBox& rhs ) const { return true; }
   bool testCollision( const BoundingSphere& rhs ) const { return true; }
   bool testCollision( const Frustum& rhs ) const { return true; }
   bool testCollision( const Ray& rhs ) const { return true; }
   bool testCollision( const Triangle& rhs ) const { return true; }
   bool testCollision( const BoundingVolume& rhs ) const { return true; }

protected:
   bool hasVolume() const { return true; }
};

///////////////////////////////////////////////////////////////////////////////
