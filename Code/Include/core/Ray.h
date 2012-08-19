/// @file   core\Ray.h
/// @file   Mathematical representation of a ray.
#pragma once

#include "core\BoundingVolume.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Mathematical representation of a ray.
 */
struct Ray : public BoundingVolume
{
   Vector origin;

   // direction should always be a normalized vector. If it's 
   // a problem (someone's changing it from outside),
   // restrict access with getters and setters
   Vector direction;

   /**
    * Default constructor.
    */
   Ray();

   /**
    * Constructor.
    *
    * @param origin
    * @param direction
    */
   Ray( const Vector& origin, const Vector& direction );

   /**
    * Returns a distance to the specified bounding volume.
    *
    * @param rhs
    */
   float getDistanceTo( const BoundingVolume& rhs ) const;

   // -------------------------------------------------------------------------
   // BoundingVolume implementation
   // -------------------------------------------------------------------------
   BoundingVolume* clone() const;
   void transform( const Matrix& mtx, BoundingVolume& transformedVolume ) const;
   void calculateBoundingBox( AABoundingBox& outBoundingBox ) const;
   PlaneClassification classifyAgainsPlane( const Plane& plane ) const;
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
