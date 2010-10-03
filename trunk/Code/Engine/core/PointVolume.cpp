#include "core\PointVolume.h"
#include "core\CollisionTests.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

PointVolume::PointVolume(const D3DXVECTOR3& _point)
: point(_point)
{
}

///////////////////////////////////////////////////////////////////////////////

BoundingVolume* PointVolume::operator*(const D3DXMATRIX& mtx) const
{
   D3DXVECTOR3 newPt;
   D3DXVec3TransformCoord(&newPt, &point, &mtx);

   return new PointVolume(newPt);
}

///////////////////////////////////////////////////////////////////////////////

void PointVolume::operator*=(const D3DXMATRIX& mtx)
{
   D3DXVec3TransformCoord(&point, &point, &mtx);
}

///////////////////////////////////////////////////////////////////////////////

float PointVolume::distanceToPlane(const D3DXPLANE& plane) const
{
   float distance = D3DXPlaneDotCoord(&plane, &point);
   return distance;
}

///////////////////////////////////////////////////////////////////////////////

bool PointVolume::testCollision(const PointVolume& rhs) const
{
   return (bool)(point == rhs.point);
}

///////////////////////////////////////////////////////////////////////////////

bool PointVolume::testCollision(const AABoundingBox& rhs) const
{
   return ::testCollision(rhs, point);
}

///////////////////////////////////////////////////////////////////////////////

bool PointVolume::testCollision(const BoundingSphere& rhs) const
{
   return ::testCollision(rhs, point);
}

///////////////////////////////////////////////////////////////////////////////

bool PointVolume::testCollision(const Frustum& rhs) const
{
   ASSERT_MSG(false, "PointVolume::testCollision(const Frustum&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool PointVolume::testCollision(const Ray& rhs) const
{
   ASSERT_MSG(false, "PointVolume::testCollision(const Ray&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool PointVolume::testCollision(const Triangle& rhs) const
{
   ASSERT_MSG(false, "PointVolume::testCollision(const Triangle&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////
