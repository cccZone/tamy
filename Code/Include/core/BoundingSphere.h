#pragma once

#include "core\BoundingVolume.h"
#include "core\Vector.h"
#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * The structure describes a sphere bounding an object
 */
ALIGN_16 struct BoundingSphere : public BoundingVolume
{
   DECLARE_ALLOCATOR( BoundingSphere, AM_ALIGNED_16 );

   Vector      origin;
   FastFloat   radius;

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
   BoundingSphere( const Vector& origin, float radius );

   /**
    * Copy constructor.
    *
    * @param rhs
    */
   BoundingSphere( const BoundingSphere& rhs );

   // -------------------------------------------------------------------------
   // BoundingVolume implementation
   // -------------------------------------------------------------------------
   BoundingVolume* clone() const;
   void transform( const Matrix& mtx, BoundingVolume& transformedVolume ) const;
   void calculateBoundingBox( AABoundingBox& outBoundingBox ) const;
   const FastFloat distanceToPlane(const Plane& plane) const;
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
