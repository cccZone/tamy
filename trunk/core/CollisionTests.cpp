#include "core\CollisionTests.h"
#include "core\AABoundingBox.h"
#include "core\Ray.h"
#include "core\BoundingSphere.h"
#include "core\Frustum.h"


///////////////////////////////////////////////////////////////////////////////

static bool areRangesOverlapping(float min1, float max1, float min2, float max2)
{
   return !((max1 < min2) || (max2 < min1));
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision(const AABoundingBox& aabb, const D3DXVECTOR3& point)
{
   return areRangesOverlapping(aabb.min.x, aabb.max.x, point.x, point.x) && 
      areRangesOverlapping(aabb.min.y, aabb.max.y, point.y, point.y) && 
      areRangesOverlapping(aabb.min.z, aabb.max.z, point.z, point.z);
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision(const AABoundingBox& aabb1, const AABoundingBox& aabb2)
{
   UINT collidingPlanes = 0;
   if (areRangesOverlapping(aabb1.min.x, aabb1.max.x, aabb2.min.x, aabb2.max.x))
   {
      collidingPlanes++;
   }

   if (areRangesOverlapping(aabb1.min.y, aabb1.max.y, aabb2.min.y, aabb2.max.y))
   {
      collidingPlanes++;
   }

   if (areRangesOverlapping(aabb1.min.z, aabb1.max.z, aabb2.min.z, aabb2.max.z))
   {
      collidingPlanes++;
   }

   return (collidingPlanes == 3);
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision(const AABoundingBox& aabb, const BoundingSphere& sphere)
{
   D3DXPLANE plane = D3DXPLANE(0, 0, -1, aabb.min.z);
   if (D3DXPlaneDotCoord(&plane, &(sphere.origin)) > sphere.radius) {return false;}

   plane = D3DXPLANE(0, 0,  1, -aabb.max.z);
   if (D3DXPlaneDotCoord(&plane, &(sphere.origin)) > sphere.radius) {return false;}

   plane = D3DXPLANE(0, -1, 0, aabb.min.y);
   if (D3DXPlaneDotCoord(&plane, &(sphere.origin)) > sphere.radius) {return false;}

   plane = D3DXPLANE(0,  1, 0, -aabb.max.y);
   if (D3DXPlaneDotCoord(&plane, &(sphere.origin)) > sphere.radius) {return false;}

   plane = D3DXPLANE(-1, 0, 0, aabb.min.x);
   if (D3DXPlaneDotCoord(&plane, &(sphere.origin)) > sphere.radius) {return false;}

   plane = D3DXPLANE( 1, 0, 0, -aabb.max.x);
   if (D3DXPlaneDotCoord(&plane, &(sphere.origin)) > sphere.radius) {return false;}
   
   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision(const Frustum& frustrum, const BoundingSphere& sphere)
{
   if (D3DXPlaneDotCoord(&(frustrum.leftPlane),  &(sphere.origin)) > sphere.radius) {return false;}
   if (D3DXPlaneDotCoord(&(frustrum.rightPlane), &(sphere.origin)) > sphere.radius) {return false;}
   if (D3DXPlaneDotCoord(&(frustrum.nearPlane),  &(sphere.origin)) > sphere.radius) {return false;}
   if (D3DXPlaneDotCoord(&(frustrum.farPlane),   &(sphere.origin)) > sphere.radius) {return false;}
   if (D3DXPlaneDotCoord(&(frustrum.upperPlane), &(sphere.origin)) > sphere.radius) {return false;}
   if (D3DXPlaneDotCoord(&(frustrum.lowerPlane), &(sphere.origin)) > sphere.radius) {return false;}

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision(const AABoundingBox& aabb, const Ray& ray)
{
   D3DXPLANE plane;
   D3DXVECTOR3 intersectionPt(0, 0, 0);

   plane = D3DXPLANE(0, 0, -1, -aabb.min.z);
   if ((testCollision(ray, plane, intersectionPt) == true) &&
       areRangesOverlapping(aabb.min.x, aabb.max.x, intersectionPt.x, intersectionPt.x) &&
       areRangesOverlapping(aabb.min.y, aabb.max.y, intersectionPt.y, intersectionPt.y))
   {
      return true;
   }

   plane = D3DXPLANE(0, 0, 1, aabb.max.z);
   if ((testCollision(ray, plane, intersectionPt) == true) &&
       areRangesOverlapping(aabb.min.x, aabb.max.x, intersectionPt.x, intersectionPt.x) &&
       areRangesOverlapping(aabb.min.y, aabb.max.y, intersectionPt.y, intersectionPt.y))
   {
      return true;
   }

   plane = D3DXPLANE(0, -1, 0, -aabb.min.y);
   if ((testCollision(ray, plane, intersectionPt) == true) &&
       areRangesOverlapping(aabb.min.x, aabb.max.x, intersectionPt.x, intersectionPt.x) &&
       areRangesOverlapping(aabb.min.z, aabb.max.z, intersectionPt.z, intersectionPt.z))
   {
      return true;
   }

   plane = D3DXPLANE(0, 1, 0, aabb.max.y);
   if ((testCollision(ray, plane, intersectionPt) == true) &&
       areRangesOverlapping(aabb.min.x, aabb.max.x, intersectionPt.x, intersectionPt.x) &&
       areRangesOverlapping(aabb.min.z, aabb.max.z, intersectionPt.z, intersectionPt.z))
   {
      return true;
   }

   plane = D3DXPLANE(-1, 0, 0, -aabb.min.x);
   if ((testCollision(ray, plane, intersectionPt) == true) &&
       areRangesOverlapping(aabb.min.z, aabb.max.z, intersectionPt.z, intersectionPt.z) &&
       areRangesOverlapping(aabb.min.y, aabb.max.y, intersectionPt.y, intersectionPt.y))
   {
      return true;
   }

   plane = D3DXPLANE(1, 0, 0, aabb.max.x);
   if ((testCollision(ray, plane, intersectionPt) == true) &&
       areRangesOverlapping(aabb.min.z, aabb.max.z, intersectionPt.z, intersectionPt.z) &&
       areRangesOverlapping(aabb.min.y, aabb.max.y, intersectionPt.y, intersectionPt.y))
   {
      return true;
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision(const Ray& ray, const D3DXPLANE& plane, D3DXVECTOR3& intersectionPt)
{
   D3DXVECTOR3 pn(plane.a, plane.b, plane.c);

   float Vd = D3DXVec3Dot(&pn, &ray.direction);
   if (Vd >= 0) return false;

   float V0 = -D3DXVec3Dot(&pn, &ray.origin) + plane.d;
   float t = V0 / Vd;
   bool isIntersecting = (V0 / Vd >= 0);

   if (isIntersecting)
   {
      intersectionPt.x = ray.origin.x + ray.direction.x * t;
      intersectionPt.y = ray.origin.y + ray.direction.y * t;
      intersectionPt.z = ray.origin.z + ray.direction.z * t;
   }

   return isIntersecting;
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision(const BoundingSphere& sphere, const D3DXVECTOR3& point)
{
   D3DXVECTOR3 to = sphere.origin - point;
   return (D3DXVec3Length(&to) < sphere.radius * sphere.radius);
}

///////////////////////////////////////////////////////////////////////////////

bool testCollision(const BoundingSphere& sphere, const BoundingSphere& rhs)
{
   D3DXVECTOR3 to = sphere.origin - rhs.origin;
   float totalRadius  = sphere.radius + rhs.radius;

   return ((&sphere != &rhs) && (D3DXVec3Length(&to) < totalRadius * totalRadius));
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 findIntersectionRemovalVector(const BoundingSphere& sphere, const BoundingSphere& colidor)
{
   D3DXVECTOR3 toColidor = colidor.origin - sphere.origin;
   float distFromEachOther = D3DXVec3Length(&toColidor);

   float amountOfOverLap = colidor.radius + sphere.radius - distFromEachOther;
   if (amountOfOverLap >= 0)
   {
      return ((toColidor / distFromEachOther) * amountOfOverLap);
   }
   else
   {
      return D3DXVECTOR3(0, 0, 0);
   }
}

///////////////////////////////////////////////////////////////////////////////


D3DXVECTOR3 findIntersectionRemovalVector(const AABoundingBox& aabb, const AABoundingBox& colidor)
{
   D3DXVECTOR3 vec(0, 0, 0);

   if (areRangesOverlapping(aabb.min.x, aabb.max.x, colidor.max.x, colidor.max.x))
   {
      vec.x = aabb.min.x - colidor.max.x;
   }
   else if (areRangesOverlapping(aabb.min.x, aabb.max.x, colidor.min.x, colidor.min.x))
   {
      vec.x = aabb.max.x - colidor.min.x;
   }

   if (areRangesOverlapping(aabb.min.z, aabb.max.z, colidor.max.z, colidor.max.z))
   {
      vec.z = aabb.min.z - colidor.max.z;
   }
   else if (areRangesOverlapping(aabb.min.z, aabb.max.z, colidor.min.z, colidor.min.z))
   {
      vec.z = aabb.max.z - colidor.min.z;
   }

   return vec;
}

///////////////////////////////////////////////////////////////////////////////
