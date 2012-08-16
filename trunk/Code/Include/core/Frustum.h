/// @file   core/Frustum.h
/// @brief  Frustum representation
#pragma once

#include "core\BoundingVolume.h"
#include "core\Plane.h"


///////////////////////////////////////////////////////////////////////////////

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

/**
 * Frustum representation.
 */
struct Frustum : public BoundingVolume
{
   Plane planes[6];

   /**
    * Calculates the bounding box of the frustum.
    *
    * @param outBB
    */
   void calculateBoundingBox( AABoundingBox& outBB ) const;

   // -------------------------------------------------------------------------
   // Bounding volume implementation
   // -------------------------------------------------------------------------
   BoundingVolume* clone() const;
   void transform( const Matrix& mtx, BoundingVolume& transformedVolume ) const;
   float distanceToPlane( const Plane& plane ) const;
   bool testCollision( const PointVolume& point ) const;
   bool testCollision( const AABoundingBox& rhs ) const;
   bool testCollision( const BoundingSphere& rhs ) const;
   bool testCollision( const Frustum& rhs ) const;
   bool testCollision( const Ray& rhs ) const;
   bool testCollision( const Triangle& rhs ) const;
   bool testCollision( const BoundingVolume& rhs ) const { return rhs.testCollision( *this ); }
};

///////////////////////////////////////////////////////////////////////////////
