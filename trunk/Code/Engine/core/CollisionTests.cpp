#include "core\CollisionTests.h"
#include "core\AABoundingBox.h"
#include "core\Ray.h"
#include "core\BoundingSphere.h"
#include "core\Frustum.h"
#include "core\Triangle.h"
#include "core\PointVolume.h"
#include "core\QuadraticEquations.h"
#include "core\Vector.h"
#include "core\Plane.h"
#include "core\FastFloat.h"
#include "core\types.h"


///////////////////////////////////////////////////////////////////////////////

static bool areRangesOverlapping( float min1, float max1, float min2, float max2 )
{
   return !( ( max1 < min2 ) || ( max2 < min1 ) );
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const AABoundingBox& aabb, const Vector& point )
{
   return areRangesOverlapping( aabb.min.x, aabb.max.x, point.x, point.x ) && 
      areRangesOverlapping( aabb.min.y, aabb.max.y, point.y, point.y ) && 
      areRangesOverlapping( aabb.min.z, aabb.max.z, point.z, point.z );
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const AABoundingBox& aabb1, const AABoundingBox& aabb2 )
{
   uint collidingPlanes = 0;
   if ( areRangesOverlapping( aabb1.min.x, aabb1.max.x, aabb2.min.x, aabb2.max.x ) )
   {
      collidingPlanes++;
   }

   if (areRangesOverlapping( aabb1.min.y, aabb1.max.y, aabb2.min.y, aabb2.max.y ) )
   {
      collidingPlanes++;
   }

   if (areRangesOverlapping( aabb1.min.z, aabb1.max.z, aabb2.min.z, aabb2.max.z ) )
   {
      collidingPlanes++;
   }

   return ( collidingPlanes == 3 );
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const AABoundingBox& aabb, const BoundingSphere& sphere )
{
   Plane plane;
   
   // <fastfloat.todo>
   FastFloat sphereRad;
   sphereRad.setFromFloat( sphere.radius );
   plane.set( Float_0, Float_0, Float_Minus1, FastFloat::fromFloat( aabb.min.z ) );
   if ( plane.dotCoord( sphere.origin ) > sphereRad ) { return false; }

   plane.set( Float_0, Float_0,  Float_1, FastFloat::fromFloat( -aabb.max.z ) );
   if ( plane.dotCoord( sphere.origin ) > sphereRad ) { return false; }

   plane.set( Float_0, Float_Minus1, Float_0, FastFloat::fromFloat( aabb.min.y ) );
   if ( plane.dotCoord( sphere.origin ) > sphereRad ) { return false; }

   plane.set( Float_0,  Float_1, Float_0, FastFloat::fromFloat( -aabb.max.y ) );
   if ( plane.dotCoord( sphere.origin ) > sphereRad ) { return false; }

   plane.set( Float_Minus1, Float_0, Float_0, FastFloat::fromFloat( aabb.min.x ) );
   if ( plane.dotCoord( sphere.origin ) > sphereRad ) { return false; }

   plane.set( Float_1, Float_0, Float_0, FastFloat::fromFloat( -aabb.max.x ) );
   if ( plane.dotCoord( sphere.origin ) > sphereRad ) { return false; }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const AABoundingBox& aabb, const Frustum& frustum )
{
   Vector pv, nv;
   for ( int i = 0; i < 6; ++i )
   {
      const Plane& plane = frustum.planes[i];

      pv.set( plane[0] > 0 ? aabb.max.x : aabb.min.x, plane[1] > 0 ? aabb.max.y : aabb.min.y, plane[2] > 0 ? aabb.max.z : aabb.min.z, 1.0f );

      const FastFloat n = plane.dotCoord( pv );
      if ( n < Float_0 )
      {
         // bounding box is outside the frustum
         return false;
      }
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const Frustum& frustum, const BoundingSphere& sphere )
{
   FastFloat negSphereRad;
   negSphereRad.setFromFloat( -sphere.radius );

   for ( int i = 0; i < 6; ++i )
   {
      const FastFloat n = frustum.planes[i].dotCoord( sphere.origin );
      if ( n < negSphereRad ) 
      {
         return false;
      }
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const Frustum& frustum, const PointVolume& point )
{
   const Vector& pt = point.point;
   for ( int i = 0; i < 6; ++i )
   {
      const FastFloat n = frustum.planes[i].dotCoord( pt );
      if ( n < Float_0 ) 
      {
         return false;
      }
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const AABoundingBox& aabb, const Ray& ray )
{
   return ( rayToAABBDistance( ray, aabb ) < FLT_MAX );
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const Ray& ray, const Plane& plane, Vector& intersectionPt )
{
   const FastFloat t = rayToPlaneDistance( ray, plane );

   if ( t >= Float_INF ) 
   { 
      return false; 
   }

   // <fastfloat.todo>
   intersectionPt.setMulAdd( ray.direction, t.getFloat(), ray.origin );

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const BoundingSphere& sphere, const Vector& point )
{
   Vector to;
   to.setSub( sphere.origin, point );
   return ( to.lengthSq() < sphere.radius * sphere.radius);
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision(const BoundingSphere& sphere, const BoundingSphere& rhs)
{
   Vector to;
   to.setSub(sphere.origin, rhs.origin );
   float totalRadius  = sphere.radius + rhs.radius;

   return ( ( &sphere != &rhs ) && ( to.lengthSq() < totalRadius * totalRadius ) );
}

///////////////////////////////////////////////////////////////////////////////

void findIntersectionRemovalVector( const BoundingSphere& sphere, const BoundingSphere& colidor, Vector& outRemovalVec )
{
   Vector toColidor;
   toColidor.setSub( colidor.origin, sphere.origin );
   float distFromEachOther = toColidor.length();

   float amountOfOverLap = colidor.radius + sphere.radius - distFromEachOther;
   if ( amountOfOverLap >= 0 )
   {
      outRemovalVec.setMul( toColidor, amountOfOverLap / distFromEachOther );
   }
   else
   {
      outRemovalVec = Vector::ZERO;
   }
}

///////////////////////////////////////////////////////////////////////////////

void findIntersectionRemovalVector( const AABoundingBox& aabb, const AABoundingBox& colidor, Vector& outRemovalVec )
{
   outRemovalVec = Vector::ZERO;

   if (areRangesOverlapping(aabb.min.x, aabb.max.x, colidor.max.x, colidor.max.x))
   {
      outRemovalVec.x = aabb.min.x - colidor.max.x;
   }
   else if (areRangesOverlapping(aabb.min.x, aabb.max.x, colidor.min.x, colidor.min.x))
   {
      outRemovalVec.x = aabb.max.x - colidor.min.x;
   }

   if (areRangesOverlapping(aabb.min.z, aabb.max.z, colidor.max.z, colidor.max.z))
   {
      outRemovalVec.z = aabb.min.z - colidor.max.z;
   }
   else if (areRangesOverlapping(aabb.min.z, aabb.max.z, colidor.min.z, colidor.min.z))
   {
      outRemovalVec.z = aabb.max.z - colidor.min.z;
   }
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const BoundingSphere& sphere, const Ray& ray )
{
   return ( rayToBSDistance( ray, sphere ) < FLT_MAX );
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const Ray& ray, const Triangle& triangle )
{
   // first check if the ray intersects the plane the triangle is on.
   Plane trianglePlane;
   trianglePlane.setFromPoints( triangle.vertexPos( 0 ), triangle.vertexPos( 1 ), triangle.vertexPos( 2 ) );

   Vector intersectionPt;
   if ( testCollision( ray, trianglePlane, intersectionPt ) == false )
   {
      return false;
   }

   // now test to see if the intersection point is located inside of the triangle
   Vector tmpDirToIntersectionPt;
   Vector tmpCrossProducts[3];
   for ( int i = 0; i < 3; ++i )
   {
      tmpDirToIntersectionPt.setSub( intersectionPt, triangle.vertexPos( i ) );
      tmpCrossProducts[i].setCross( tmpDirToIntersectionPt, triangle.edge( i ) );
   }

   for ( int i = 0; i < 3; ++i )
   {
      int nextIdx = ( i + 1 ) % 3;
      float dot = tmpCrossProducts[i].dot( tmpCrossProducts[nextIdx] );
      if ( dot < 0 )
      {
         // the point lies outside the triangle
         return false;
      }
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

const FastFloat rayToPlaneDistance( const Ray& ray, const Plane& plane )
{
   const FastFloat planeDistance = plane.dotCoord( ray.origin );
   const FastFloat projRayLength = plane.dotNormal( ray.direction );

   FastFloat t;
   if ( projRayLength == Float_0 )
   {
      if ( planeDistance == Float_0 )
      {
         t = Float_0;
      }
      else
      {
         t = Float_Minus1;
      }
   }
   else
   {
      t.setDiv(  planeDistance, projRayLength );
      t.neg();
   }

   if ( t < Float_0 ) 
   {
      return Float_INF;
   }
   else
   {
      return t;
   }
}

///////////////////////////////////////////////////////////////////////////////

float rayToPointDistance( const Ray& ray, const Vector& point )
{
   Vector vecToPt;
   vecToPt.setSub( point, ray.origin );

   float t = vecToPt.dot( ray.direction );
   if (t < 0)
   {
      // if the point lies behind the ray's origin, the distance is infinite
      return FLT_MAX;
   }

   // check if the projected point lies on the ray
   Vector resultPt;
   resultPt.setMulAdd( ray.direction, t, ray.origin ).sub( vecToPt );
   if ( resultPt.lengthSq() < 1e-3 )
   {
      // if it does - return the calculated distance
      return t;
   }
   else
   {
      // if not - it's not on the ray, so the distance to it is infinite
      return FLT_MAX;
   }
}

///////////////////////////////////////////////////////////////////////////////

float rayToAABBDistance(const Ray& ray, const AABoundingBox& aabb)
{
   // 'slabs' algorithm
   float tMin = -FLT_MAX;
   float tMax = FLT_MAX;
   float t1, t2, fTemp;

   Vector extentsMin;
   extentsMin.setSub( aabb.min, ray.origin );
   Vector extentsMax;
   extentsMax.setSub( aabb.max, ray.origin );

   for (int i = 0; i < 3; ++i)
   {
      if ( fabsf( ray.direction[i] ) > 1e-3f )
      {
         fTemp = 1.0f / ray.direction[i];
         t1 = extentsMax[i] * fTemp;
         t2 = extentsMin[i] * fTemp;
         if (t1 > t2) {fTemp = t1; t1 = t2; t2 = fTemp; }

         if ( t1 > tMin ) 
         {
            tMin = t1;
         }
         if ( t2 < tMax )
         {
            tMax = t2;
         }

         if ( tMin > tMax )
         {
            return FLT_MAX;
         }
         if ( tMax < 0 )
         {
            return FLT_MAX;
         }
      }
      else
      {
         if ( ( ray.origin[i] < aabb.min[i] ) || ( ray.origin[i] > aabb.max[i] ) )
         {
            return FLT_MAX;
         }
      }
   }

   if ( tMin < 0 )
   {
      tMin = 0;
   }
   return tMin;
}

///////////////////////////////////////////////////////////////////////////////

float rayToBSDistance( const Ray& ray, const BoundingSphere& bs )
{
   float a, b, c, l2, t;

   Vector L;
   L.setSub( ray.origin, bs.origin );
   l2 = L.lengthSq();

   a = ray.direction.lengthSq();
   b = 2.0f * ray.direction.dot( L );
   c = l2 - ( bs.radius * bs.radius );

   if ( c < 0.0f ) 
   {
      // the ray starts inside the sphere
      return 0;
   }

   if ( !solveQuadratic( a, b, c, t ) ) 
   {
      // the sphere doesn't intersect the plane
      return FLT_MAX;
   }

   return t;
}

///////////////////////////////////////////////////////////////////////////////
