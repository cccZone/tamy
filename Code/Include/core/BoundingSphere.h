#pragma once

#include "core\BoundingVolume.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * The structure describes a sphere bounding an object
 */
struct BoundingSphere : public BoundingVolume
{
   Vector      origin;
   float       radius;

   /**
    * Default constructor.
    */
   BoundingSphere();

   /**
    * Constructor.
    *
    * @param origin
    * @param radius
    */
   BoundingSphere(const Vector& origin, float radius);

   // -------------------------------------------------------------------------
   // BoundingVolume implementation
   // -------------------------------------------------------------------------
   BoundingVolume* clone() const;
   void transform( const Matrix& mtx, BoundingVolume& transformedVolume ) const;
   float distanceToPlane(const Plane& plane) const;
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
