#pragma once

/// @file   core\AABoundingBox.h
/// @brief  an axis aligned bounding box

#include "core\BoundingVolume.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

struct Matrix;
struct Vector;
struct Plane;

///////////////////////////////////////////////////////////////////////////////

/**
 * the structure describes an axis aligned bounding box
 */
struct AABoundingBox : public BoundingVolume
{
   Vector min;
   Vector max;

   /**
    * Constructor.
    */
   AABoundingBox();

   /**
    * Constructor.
    *
    * @param min
    * @param max
    */
   AABoundingBox( const Vector& min, const Vector& max );

   /**
    * Union of two boxes.
    *
    * @param otherBox
    * @param unionBox
    */
   void add( const AABoundingBox& otherBox, AABoundingBox& unionBox ) const; 

   /**
    * This method will modify the bounding box such that it includes
    * the specified point.
    *
    * @param pt   point we want to bound inside the box
    */
   void include( const Vector& pt );

   // -------------------------------------------------------------------------
   // BoundingVolume implementation
   // -------------------------------------------------------------------------
   BoundingVolume* clone() const;
   void transform( const Matrix& mtx, BoundingVolume& transformedVolume ) const;
   float distanceToPlane( const Plane& plane ) const;
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
