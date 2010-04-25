#include "core\Ray.h"
#include "core\Assert.h"
#include "core\CollisionTests.h"
#include "core\PointVolume.h"
#include "core\AABoundingBox.h"
#include "core\BoundingSphere.h"
#include "core\QuadraticEquations.h"


///////////////////////////////////////////////////////////////////////////////

BoundingVolume* Ray::operator*(const D3DXMATRIX& mtx) const
{
   Ray* newRay = new Ray();

   D3DXVec3TransformCoord(&newRay->origin, &origin, &mtx);
   D3DXVec3TransformNormal(&newRay->direction, &direction, &mtx);

   return newRay;
}

///////////////////////////////////////////////////////////////////////////////

void Ray::operator*=(const D3DXMATRIX& mtx)
{
   D3DXVec3TransformCoord(&origin, &origin, &mtx);
   D3DXVec3TransformNormal(&direction, &direction, &mtx);
}

///////////////////////////////////////////////////////////////////////////////

PlaneClassification Ray::classifyAgainsPlane(const D3DXPLANE& plane) const
{
   ASSERT(false, "Ray::classifyAgainsPlane(const D3DXPLANE&) - Method not implemented");
   return PPC_SPANNING;
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

float Ray::getDistanceTo(const BoundingVolume& rhs) const
{
   if (dynamic_cast<const PointVolume*> (&rhs) != NULL)
   {
      return rayToPointDistance(*this, (dynamic_cast<const PointVolume*> (&rhs))->point);
   }
   else if (dynamic_cast<const AABoundingBox*> (&rhs) != NULL)
   {
      return rayToAABBDistance(*this, *(dynamic_cast<const AABoundingBox*> (&rhs)));
   }
   else if (dynamic_cast<const BoundingSphere*> (&rhs) != NULL)
   {
      return rayToBSDistance(*this, *(dynamic_cast<const BoundingSphere*> (&rhs)));
   }
   else
   {
      ASSERT(false, "Ray::getDistanceTo(...) not implemented for this type of bounding volume");
      return FLT_MAX;
   }
}

///////////////////////////////////////////////////////////////////////////////

float Ray::distanceToPlane(const D3DXPLANE& plane) const
{
   return rayToPlaneDistance(*this, plane);
}

///////////////////////////////////////////////////////////////////////////////
