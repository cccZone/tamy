#include "core\PointVolume.h"
#include "core\CollisionTests.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

PointVolume::PointVolume(const D3DXVECTOR3& _point)
: point(_point)
{
}

///////////////////////////////////////////////////////////////////////////////

BoundingVolume* PointVolume::clone() const
{
   return new PointVolume( point );
}

///////////////////////////////////////////////////////////////////////////////

void PointVolume::transform( const D3DXMATRIX& mtx, BoundingVolume& transformedVolume ) const
{
   // verify that the volume is a PointVolume
   ASSERT( dynamic_cast< PointVolume* >( &transformedVolume ) != NULL );
   PointVolume& transformedPoint = static_cast< PointVolume& >( transformedVolume );

   D3DXVec3TransformCoord( &transformedPoint.point, &point, &mtx );
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
   // TODO: implement me
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
