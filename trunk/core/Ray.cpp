#include "core\Ray.h"
#include "core\Assert.h"
#include "core\CollisionTests.h"


///////////////////////////////////////////////////////////////////////////////

BoundingVolume* Ray::operator*(const D3DXMATRIX& mtx) const
{
   Ray* newRay = new Ray();

   D3DXVec3TransformCoord(&newRay->origin, &origin, &mtx);
   D3DXVec3TransformNormal(&newRay->direction, &direction, &mtx);

   return newRay;
}


///////////////////////////////////////////////////////////////////////////////

bool Ray::testCollision(const PointVolume& point) const
{
   ASSERT(false, "Ray::testCollision(const PointVolume&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Ray::testCollision(const AABoundingBox& rhs) const
{
   return ::testCollision(rhs, *this);
}

///////////////////////////////////////////////////////////////////////////////

bool Ray::testCollision(const BoundingSphere& rhs) const
{
   return ::testCollision(rhs, *this);
}

///////////////////////////////////////////////////////////////////////////////

bool Ray::testCollision(const Frustum& rhs) const
{
   ASSERT(false, "Ray::testCollision(const Frustum&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Ray::testCollision(const Ray& rhs) const
{
   ASSERT(false, "Ray::testCollision(const Frustum&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Ray::testCollision(const Triangle& rhs) const
{
   return ::testCollision(*this, rhs);
}

///////////////////////////////////////////////////////////////////////////////
