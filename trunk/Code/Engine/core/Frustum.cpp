#include "core\Frustum.h"
#include "core\Assert.h"
#include "core\CollisionTests.h"
#include "core\Matrix.h"
#include "core\AABoundingBox.h"
#include "core\PlaneUtils.h"


///////////////////////////////////////////////////////////////////////////////

BoundingVolume* Frustum::clone() const
{
   return new Frustum( *this );
}

///////////////////////////////////////////////////////////////////////////////

void Frustum::transform( const Matrix& mtx, BoundingVolume& transformedVolume ) const
{
   // verify that the volume is a Frustum
   ASSERT( dynamic_cast< Frustum* >( &transformedVolume ) != NULL );
   Frustum& transformedFrustum = static_cast< Frustum& >( transformedVolume );

   for (char i = 0; i < 6; ++i)
   {
      mtx.transform( planes[i], transformedFrustum.planes[i] );
   }
}

///////////////////////////////////////////////////////////////////////////////

float Frustum::distanceToPlane( const Plane& plane ) const
{
   ASSERT_MSG(false, "Frustum::distanceToPlane(const Plane&) - Method not implemented");
   return 0;
}

///////////////////////////////////////////////////////////////////////////////

bool Frustum::testCollision(const PointVolume& point ) const
{
   return ::testCollision( *this, point );
}

///////////////////////////////////////////////////////////////////////////////

bool Frustum::testCollision(const AABoundingBox& rhs) const
{
   return ::testCollision(rhs, *this);
}

///////////////////////////////////////////////////////////////////////////////

bool Frustum::testCollision(const BoundingSphere& rhs) const
{
   return ::testCollision(*this, rhs);
}

///////////////////////////////////////////////////////////////////////////////

bool Frustum::testCollision(const Frustum& rhs) const
{
   ASSERT_MSG(false, "Frustum::testCollision(const Frustum&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Frustum::testCollision(const Ray& rhs) const
{
   ASSERT_MSG(false, "Frustum::testCollision(const Ray&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Frustum::testCollision(const Triangle& rhs) const
{
   ASSERT_MSG(false, "Frustum::testCollision(const Triangle&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

void Frustum::calculateBoundingBox( AABoundingBox& outBB ) const
{
   // TODO: !!!!!!! this is extremely slow - speed it up
   Vector points[8];
   PlaneUtils::calculatePlanesIntersection( planes[FP_LEFT], planes[FP_TOP], planes[FP_NEAR], points[0] );
   PlaneUtils::calculatePlanesIntersection( planes[FP_RIGHT], planes[FP_TOP], planes[FP_NEAR], points[1] );
   PlaneUtils::calculatePlanesIntersection( planes[FP_LEFT], planes[FP_BOTTOM], planes[FP_NEAR], points[2] );
   PlaneUtils::calculatePlanesIntersection( planes[FP_RIGHT], planes[FP_BOTTOM], planes[FP_NEAR], points[3] );
   PlaneUtils::calculatePlanesIntersection( planes[FP_LEFT], planes[FP_TOP], planes[FP_FAR], points[4] );
   PlaneUtils::calculatePlanesIntersection( planes[FP_RIGHT], planes[FP_TOP], planes[FP_FAR], points[5] );
   PlaneUtils::calculatePlanesIntersection( planes[FP_LEFT], planes[FP_BOTTOM], planes[FP_FAR], points[6] );
   PlaneUtils::calculatePlanesIntersection( planes[FP_RIGHT], planes[FP_BOTTOM], planes[FP_FAR], points[7] );

   outBB.min.set( FLT_MAX, FLT_MAX, FLT_MAX );
   outBB.max.set( -FLT_MAX, -FLT_MAX, -FLT_MAX );
   for ( char i = 0; i < 8; ++ i )
   {
      outBB.include( points[i] );
   }
}

///////////////////////////////////////////////////////////////////////////////

