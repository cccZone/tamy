#include "core\Frustum.h"
#include "core\Assert.h"
#include "core\CollisionTests.h"


///////////////////////////////////////////////////////////////////////////////

BoundingVolume* Frustum::clone() const
{
   return new Frustum( *this );
}

///////////////////////////////////////////////////////////////////////////////

void Frustum::transform( const D3DXMATRIX& mtx, BoundingVolume& transformedVolume ) const
{
   // verify that the volume is a Frustum
   ASSERT( dynamic_cast< Frustum* >( &transformedVolume ) != NULL );
   Frustum& transformedFrustum = static_cast< Frustum& >( transformedVolume );

   for (char i = 0; i < 6; ++i)
   {
      D3DXPlaneTransform( &transformedFrustum.planes[i], &planes[i], &mtx );
   }
}

///////////////////////////////////////////////////////////////////////////////

float Frustum::distanceToPlane(const D3DXPLANE& plane) const
{
   ASSERT_MSG(false, "Frustum::distanceToPlane(const D3DXPLANE&) - Method not implemented");
   return 0;
}

///////////////////////////////////////////////////////////////////////////////

bool Frustum::testCollision(const PointVolume& point) const
{
   ASSERT_MSG(false, "Frustum::testCollision(const PointVolume&) - Method not implemented");
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