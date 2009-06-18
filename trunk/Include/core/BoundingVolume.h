#pragma once


///////////////////////////////////////////////////////////////////////////////

struct AABoundingBox;
struct BoundingSphere;
struct Frustum;
struct Ray;
struct Triangle;
struct D3DXVECTOR3;
struct D3DXMATRIX;

///////////////////////////////////////////////////////////////////////////////

/**
 * This interface allows to check collisions between all available collision shapes
 */
class BoundingVolume
{
public:
   virtual ~BoundingVolume() {}

   virtual bool testCollision(const D3DXVECTOR3& point) const = 0;

   virtual bool testCollision(const AABoundingBox& rhs) const = 0;

   virtual bool testCollision(const BoundingSphere& rhs) const = 0;

   virtual bool testCollision(const Frustum& rhs) const = 0;

   virtual bool testCollision(const Ray& rhs) const = 0;

   virtual bool testCollision(const Triangle& rhs) const = 0;

   virtual bool testCollision(const BoundingVolume& rhs) const = 0;

   virtual BoundingVolume* operator*(const D3DXMATRIX& mtx) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
