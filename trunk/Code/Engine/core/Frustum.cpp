#include "core\Frustum.h"
#include "core\Assert.h"
#include "core\CollisionTests.h"
#include "core\Matrix.h"
#include "core\AABoundingBox.h"


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

bool Frustum::testCollision(const PointVolume& point) const
{
   // <math.todo>: implement me
   return false;
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

// TODO: move this method to a utility class
static bool calculatePlanesIntersection( const Plane& p1, const Plane& p2, const Plane& p3, Vector& outIntersectionPt )
{
   Vector n1, n2, n3;
   p1.getNormal( n1 );
   p2.getNormal( n2 );
   p3.getNormal( n3 );

   Vector n23, n31, n12;
   n23.setCross( n2, n3 );
   n31.setCross( n3, n1 );
   n12.setCross( n1, n2 );

   float den = -p1.dotNormal( n23 );
   if ( den == 0 )
   {
      // planes are parallel
      return false;
   }

   Vector a, b, c;
   a.setMul( n23, p1.d );
   b.setMul( n31, p2.d );
   c.setMul( n12, p3.d );

   outIntersectionPt.setAdd( a, b ).add( c ).mul( 1.0f / den );
   return true;
}

void Frustum::calculateBoundingBox( AABoundingBox& outBB ) const
{
   // TODO: !!!!!!! this is extremely slow - speed it up
   Vector points[8];
   calculatePlanesIntersection( planes[FP_LEFT], planes[FP_TOP], planes[FP_NEAR], points[0] );
   calculatePlanesIntersection( planes[FP_RIGHT], planes[FP_TOP], planes[FP_NEAR], points[1] );
   calculatePlanesIntersection( planes[FP_LEFT], planes[FP_BOTTOM], planes[FP_NEAR], points[2] );
   calculatePlanesIntersection( planes[FP_RIGHT], planes[FP_BOTTOM], planes[FP_NEAR], points[3] );
   calculatePlanesIntersection( planes[FP_LEFT], planes[FP_TOP], planes[FP_FAR], points[4] );
   calculatePlanesIntersection( planes[FP_RIGHT], planes[FP_TOP], planes[FP_FAR], points[5] );
   calculatePlanesIntersection( planes[FP_LEFT], planes[FP_BOTTOM], planes[FP_FAR], points[6] );
   calculatePlanesIntersection( planes[FP_RIGHT], planes[FP_BOTTOM], planes[FP_FAR], points[7] );

   outBB.min.set( FLT_MAX, FLT_MAX, FLT_MAX );
   outBB.max.set( -FLT_MAX, -FLT_MAX, -FLT_MAX );
   for ( char i = 0; i < 8; ++ i )
   {
      outBB.include( points[i] );
   }
}

///////////////////////////////////////////////////////////////////////////////

