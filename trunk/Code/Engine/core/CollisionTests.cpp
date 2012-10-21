#include "core.h"
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
   // <fastfloat.todo> areRangesOverlapping - this can be done using a selector
   return areRangesOverlapping( aabb.min[0], aabb.max[0], point[0], point[0] ) && 
      areRangesOverlapping( aabb.min[1], aabb.max[1], point[1], point[1] ) && 
      areRangesOverlapping( aabb.min[2], aabb.max[2], point[2], point[2] );
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const AABoundingBox& aabb1, const AABoundingBox& aabb2 )
{
   // <fastfloat.todo> areRangesOverlapping - this can be done using a selector

   uint collidingPlanes = 0;
   if ( areRangesOverlapping( aabb1.min[0], aabb1.max[0], aabb2.min[0], aabb2.max[0] ) )
   {
      collidingPlanes++;
   }

   if (areRangesOverlapping( aabb1.min[1], aabb1.max[1], aabb2.min[1], aabb2.max[1] ) )
   {
      collidingPlanes++;
   }

   if (areRangesOverlapping( aabb1.min[2], aabb1.max[2], aabb2.min[2], aabb2.max[2] ) )
   {
      collidingPlanes++;
   }

   return ( collidingPlanes == 3 );
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const AABoundingBox& aabb, const BoundingSphere& sphere )
{
   Plane plane;
   
   FastFloat sphereRad = sphere.radius;
   plane.set( Float_0, Float_0, Float_Minus1, FastFloat::fromFloat( aabb.min[2] ) );
   if ( plane.dotCoord( sphere.origin ) > sphereRad ) { return false; }

   plane.set( Float_0, Float_0,  Float_1, FastFloat::fromFloat( -aabb.max[2] ) );
   if ( plane.dotCoord( sphere.origin ) > sphereRad ) { return false; }

   plane.set( Float_0, Float_Minus1, Float_0, FastFloat::fromFloat( aabb.min[1] ) );
   if ( plane.dotCoord( sphere.origin ) > sphereRad ) { return false; }

   plane.set( Float_0,  Float_1, Float_0, FastFloat::fromFloat( -aabb.max[1] ) );
   if ( plane.dotCoord( sphere.origin ) > sphereRad ) { return false; }

   plane.set( Float_Minus1, Float_0, Float_0, FastFloat::fromFloat( aabb.min[0] ) );
   if ( plane.dotCoord( sphere.origin ) > sphereRad ) { return false; }

   plane.set( Float_1, Float_0, Float_0, FastFloat::fromFloat( -aabb.max[0] ) );
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

      pv.set( plane[0] > 0 ? aabb.max[0] : aabb.min[0], plane[1] > 0 ? aabb.max[1] : aabb.min[1], plane[2] > 0 ? aabb.max[2] : aabb.min[2], 1.0f );

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
   negSphereRad.setNeg( sphere.radius );

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
   return ( rayToAABBDistance( ray, aabb ) < Float_INF );
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
   intersectionPt.setMulAdd( ray.direction, t, ray.origin );

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const BoundingSphere& sphere, const Vector& point )
{
   Vector to;
   to.setSub( sphere.origin, point );

   FastFloat radiusSq;
   radiusSq.setMul( sphere.radius, sphere.radius );

   return ( to.lengthSq() < radiusSq );
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision(const BoundingSphere& sphere, const BoundingSphere& rhs)
{
   Vector to;
   to.setSub(sphere.origin, rhs.origin );
   FastFloat totalRadius;
   totalRadius.setAdd( sphere.radius, rhs.radius );
   totalRadius.mul( totalRadius );

   return ( ( &sphere != &rhs ) && ( to.lengthSq() < totalRadius ) );
}

///////////////////////////////////////////////////////////////////////////////

void findIntersectionRemovalVector( const BoundingSphere& sphere, const BoundingSphere& colidor, Vector& outRemovalVec )
{
   Vector toColidor;
   toColidor.setSub( colidor.origin, sphere.origin );
   const FastFloat distFromEachOther = toColidor.length();

   FastFloat amountOfOverLap;
   amountOfOverLap.setAdd( colidor.radius, sphere.radius );
   amountOfOverLap.sub( distFromEachOther );
   if ( amountOfOverLap >= Float_0 )
   {
      amountOfOverLap.div( distFromEachOther );
      outRemovalVec.setMul( toColidor, amountOfOverLap );
   }
   else
   {
      outRemovalVec = Quad_0;
   }
}

///////////////////////////////////////////////////////////////////////////////

void findIntersectionRemovalVector( const AABoundingBox& aabb, const AABoundingBox& colidor, Vector& outRemovalVec )
{
   outRemovalVec = Quad_0;

   // <fastfloat.todo> areRangesOverlapping - this can be done using a selector

   if (areRangesOverlapping(aabb.min[0], aabb.max[0], colidor.max[0], colidor.max[0]))
   {
      outRemovalVec[0] = aabb.min[0] - colidor.max[0];
   }
   else if (areRangesOverlapping(aabb.min[0], aabb.max[0], colidor.min[0], colidor.min[0]))
   {
      outRemovalVec[0] = aabb.max[0] - colidor.min[0];
   }

   if (areRangesOverlapping(aabb.min[1], aabb.max[1], colidor.max[1], colidor.max[1]))
   {
      outRemovalVec[1] = aabb.min[1] - colidor.max[1];
   }
   else if (areRangesOverlapping(aabb.min[1], aabb.max[1], colidor.min[1], colidor.min[1]))
   {
      outRemovalVec[1] = aabb.max[1] - colidor.min[1];
   }

   if (areRangesOverlapping(aabb.min[2], aabb.max[2], colidor.max[2], colidor.max[2]))
   {
      outRemovalVec[2] = aabb.min[2] - colidor.max[2];
   }
   else if (areRangesOverlapping(aabb.min[2], aabb.max[2], colidor.min[2], colidor.min[2]))
   {
      outRemovalVec[2] = aabb.max[2] - colidor.min[2];
   }
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision( const BoundingSphere& sphere, const Ray& ray )
{
   return ( rayToBSDistance( ray, sphere ) < Float_INF );
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
      const FastFloat dot = tmpCrossProducts[i].dot( tmpCrossProducts[nextIdx] );
      if ( dot < Float_0 )
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

const FastFloat rayToPointDistance( const Ray& ray, const Vector& point )
{
   Vector vecToPt;
   vecToPt.setSub( point, ray.origin );

   const FastFloat t = vecToPt.dot( ray.direction );
   if ( t < Float_0 )
   {
      // if the point lies behind the ray's origin, the distance is infinite
      return Float_INF;
   }

   // check if the projected point lies on the ray
   Vector resultPt;
   resultPt.setMulAdd( ray.direction, t, ray.origin );
   resultPt.sub( vecToPt );
   if ( resultPt.lengthSq() < Float_1e_3 )
   {
      // if it does - return the calculated distance
      return t;
   }
   else
   {
      // if not - it's not on the ray, so the distance to it is infinite
      return Float_INF;
   }
}

///////////////////////////////////////////////////////////////////////////////

const FastFloat rayToAABBDistance(const Ray& ray, const AABoundingBox& aabb)
{
   // 'slabs' algorithm
   FastFloat tMin; tMin.setNeg( Float_INF );
   FastFloat tMax = Float_INF;
   FastFloat t1, t2, fTemp;

   Vector extentsMin;
   extentsMin.setSub( aabb.min, ray.origin );
   Vector extentsMax;
   extentsMax.setSub( aabb.max, ray.origin );

   // <fastfloat> this should also be feasible to do using selectors
   FastFloat dir, dirAbs, dirComp;
   for (int i = 0; i < 3; ++i)
   {
      dirComp = ray.direction.getComponent( i );
      dirAbs.setAbs( dirComp );
      if ( dirAbs > Float_1e_3 )
      {
         fTemp.setReciprocal( dirComp );
         t1.setMul( extentsMax.getComponent( i ), fTemp );
         t2.setMul( extentsMin.getComponent( i ), fTemp );
         if (t1 > t2) 
         {
            fTemp = t1; 
            t1 = t2; 
            t2 = fTemp; 
         }

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
            return Float_INF;
         }
         if ( tMax < Float_0 )
         {
            return Float_INF;
         }
      }
      else
      {
         if ( ( ray.origin[i] < aabb.min[i] ) || ( ray.origin[i] > aabb.max[i] ) )
         {
            return Float_INF;
         }
      }
   }

   if ( tMin < Float_0 )
   {
      tMin = Float_0;
   }
   return tMin;
}

///////////////////////////////////////////////////////////////////////////////

const FastFloat rayToBSDistance( const Ray& ray, const BoundingSphere& bs )
{
   FastFloat a, b, c, l2;

   Vector L;
   L.setSub( ray.origin, bs.origin );
   l2 = L.lengthSq();

   a = ray.direction.lengthSq();
   b.setMul( Float_2, ray.direction.dot( L ) );

   c.setMul( bs.radius, bs.radius );
   c.neg();
   c.add( l2 );

   if ( c < Float_0 ) 
   {
      // the ray starts inside the sphere
      return Float_0;
   }

   float t;
   if ( !solveQuadratic( a.getFloat(), b.getFloat(), c.getFloat(), t ) ) 
   {
      // the sphere doesn't intersect the plane
      return Float_INF;
   }

   return FastFloat::fromFloat( t );
}

///////////////////////////////////////////////////////////////////////////////
