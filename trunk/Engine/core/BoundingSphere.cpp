#include "core\BoundingSphere.h"
#include "core\Assert.h"
#include "core\CollisionTests.h"
#include "core\PointVolume.h"


///////////////////////////////////////////////////////////////////////////////

BoundingSphere::BoundingSphere()
      : origin(0, 0, 0),
      radius(0)
{
}

///////////////////////////////////////////////////////////////////////////////

BoundingSphere::BoundingSphere(const D3DXVECTOR3& _origin, float _radius)
      : origin(_origin),
      radius(_radius)
{
}

///////////////////////////////////////////////////////////////////////////////

BoundingVolume* BoundingSphere::operator*(const D3DXMATRIX& mtx) const
{
   BoundingSphere* newSphere = new BoundingSphere();

   D3DXVec3TransformCoord(&newSphere->origin, &origin, &mtx);
   newSphere->radius = radius;

   return newSphere;
}

///////////////////////////////////////////////////////////////////////////////

bool BoundingSphere::testCollision(const PointVolume& rhs) const
{
   return ::testCollision(*this, rhs.point);
}

///////////////////////////////////////////////////////////////////////////////

bool BoundingSphere::testCollision(const AABoundingBox& rhs) const
{
   return ::testCollision(rhs, *this);
}

///////////////////////////////////////////////////////////////////////////////

bool BoundingSphere::testCollision(const BoundingSphere& rhs) const
{
   return ::testCollision(*this, rhs);
}

///////////////////////////////////////////////////////////////////////////////

bool BoundingSphere::testCollision(const Frustum& rhs) const
{
   return ::testCollision(rhs, *this);
}

///////////////////////////////////////////////////////////////////////////////

bool BoundingSphere::testCollision(const Ray& rhs) const
{
   return ::testCollision(*this, rhs);
}

///////////////////////////////////////////////////////////////////////////////

bool BoundingSphere::testCollision(const Triangle& rhs) const
{
   ASSERT(false, "BoundingSphere::testCollision(const Triangle&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////
