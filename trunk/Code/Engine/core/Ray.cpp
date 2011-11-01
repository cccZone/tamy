#include "core\Ray.h"
#include "core\Assert.h"
#include "core\CollisionTests.h"
#include "core\PointVolume.h"
#include "core\AABoundingBox.h"
#include "core\BoundingSphere.h"
#include "core\QuadraticEquations.h"


///////////////////////////////////////////////////////////////////////////////

Ray::Ray() 
   : origin( 0, 0, 0 )
   , direction( 0, 0, 1 ) 
{}

///////////////////////////////////////////////////////////////////////////////
Ray::Ray( const D3DXVECTOR3& _origin, const D3DXVECTOR3& _direction )
   : origin( _origin )
   , direction( _direction )
{
   D3DXVec3Normalize( &direction, &direction );
}

///////////////////////////////////////////////////////////////////////////////

BoundingVolume* Ray::clone() const
{
   return new Ray( origin, direction );
}

///////////////////////////////////////////////////////////////////////////////

void Ray::transform( const D3DXMATRIX& mtx, BoundingVolume& transformedVolume ) const
{
   // verify that the volume is a Ray
   ASSERT( dynamic_cast< Ray* >( &transformedVolume ) != NULL );
   Ray& transformedRay = static_cast< Ray& >( transformedVolume );

   D3DXVec3TransformCoord( &transformedRay.origin, &origin, &mtx );
   D3DXVec3TransformNormal( &transformedRay.direction, &direction, &mtx );
}

///////////////////////////////////////////////////////////////////////////////

PlaneClassification Ray::classifyAgainsPlane(const D3DXPLANE& plane) const
{
   ASSERT_MSG(false, "Ray::classifyAgainsPlane(const D3DXPLANE&) - Method not implemented");
   return PPC_SPANNING;
}

///////////////////////////////////////////////////////////////////////////////

bool Ray::testCollision(const PointVolume& point) const
{
   ASSERT_MSG(false, "Ray::testCollision(const PointVolume&) - Method not implemented");
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
   ASSERT_MSG(false, "Ray::testCollision(const Frustum&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Ray::testCollision(const Ray& rhs) const
{
   ASSERT_MSG(false, "Ray::testCollision(const Frustum&) - Method not implemented");
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
      ASSERT_MSG(false, "Ray::getDistanceTo(...) not implemented for this type of bounding volume");
      return FLT_MAX;
   }
}

///////////////////////////////////////////////////////////////////////////////

float Ray::distanceToPlane(const D3DXPLANE& plane) const
{
   return rayToPlaneDistance(*this, plane);
}

///////////////////////////////////////////////////////////////////////////////
