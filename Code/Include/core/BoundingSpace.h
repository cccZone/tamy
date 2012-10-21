/// @file   core/BoundingSpace.h
/// @brief  an infinite space bounding volume
#pragma once

#include "core\BoundingVolume.h"
#include "core\AABoundingBox.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Space is infinite - thus is this volume. It should be used
 * to bound things that should be considered ubiquitous,
 * like directional and ambient lighting
 */
struct BoundingSpace : public BoundingVolume
{
   DECLARE_ALLOCATOR( BoundingSpace, AM_ALIGNED_16 );

   // -------------------------------------------------------------------------
   // BoundingVolume implementation
   // -------------------------------------------------------------------------
   BoundingVolume* clone() const { return new BoundingSpace(); }
   void transform( const Matrix& mtx, BoundingVolume& transformedVolume ) const {}
   void calculateBoundingBox( AABoundingBox& outBoundingBox ) const { outBoundingBox.min.set( -FLT_MAX, -FLT_MAX, -FLT_MAX ); outBoundingBox.max.set( FLT_MAX, FLT_MAX, FLT_MAX ); }
   const FastFloat distanceToPlane( const Plane& plane ) const { return Float_0; }
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
