#pragma once

/// @file   core\BoundingVolume.h
/// @brief  common interface for inter-bounding volume collision tests

///////////////////////////////////////////////////////////////////////////////

struct AABoundingBox;
struct BoundingSphere;
struct Frustum;
struct Ray;
struct Triangle;
struct BoundingSpace;
struct D3DXMATRIX;
struct PointVolume;
struct D3DXPLANE;

///////////////////////////////////////////////////////////////////////////////

enum PlaneClassification
{
    PPC_BACK,
    PPC_FRONT,
    PPC_SPANNING,
    PPC_COPLANAR
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This interface allows to check collisions between all available collision shapes
 */
class BoundingVolume
{
public:
   virtual ~BoundingVolume() {}

   /**
    * This method checks on which side of the specified plane
    * is the volume situated.
    *
    * @param plane   plane against which we want to perform a check
    * @return        position of the volume in relation to the plane
    */
   virtual PlaneClassification classifyAgainsPlane(const D3DXPLANE& plane) const;

   /**
    * This method checks on which side of the specified plane
    * is the volume situated.
    *
    * @param plane   plane against which we want to perform a check
    * @return        <0 - the volume is behind the plane
    *                =0 - the volume intersects the plane
    *                >0 - the volume is in front of the plane
    */
   virtual float distanceToPlane(const D3DXPLANE& plane) const = 0;

   virtual bool testCollision(const PointVolume& point) const = 0;

   virtual bool testCollision(const AABoundingBox& rhs) const = 0;

   virtual bool testCollision(const BoundingSphere& rhs) const = 0;

   virtual bool testCollision(const Frustum& rhs) const = 0;

   virtual bool testCollision(const Ray& rhs) const = 0;

   virtual bool testCollision(const Triangle& rhs) const = 0;

   /**
    * Collision with a bounding space always exists, as the space is
    * infinite and ubiquitous
    */
   bool testCollision(const BoundingSpace& rhs) const {return true;}

   virtual bool testCollision(const BoundingVolume& rhs) const = 0;

   virtual BoundingVolume* operator*(const D3DXMATRIX& mtx) const = 0;

protected:
   /**
    * The method should return true if an instance has a non-zero volume.
    * I.e. points and rays don't have a volume, so they will always return
    * false, but an axis aligned box can return false only if both
    * its extents represent the same point in space.
    *
    * @return     true if the instance has a non-zero volume, false otherwise
    */
   virtual bool hasVolume() const {return false;}
};

///////////////////////////////////////////////////////////////////////////////
