#include "core\Frustum.h"
#include "core\Assert.h"
#include "core\CollisionTests.h"
#include "core\Matrix.h"


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
   // TODO: implement me
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
