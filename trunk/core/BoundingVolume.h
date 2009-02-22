#pragma once

#include <d3dx9.h>


struct Ray;

//-----------------------------------------------------------------------------
// (helper struct)
// the structure describes an axis aligned bounding box of a mesh
//-----------------------------------------------------------------------------
struct AABoundingBox
{
   D3DXVECTOR3 min;
   D3DXVECTOR3 max;

   AABoundingBox();
   AABoundingBox(const D3DXVECTOR3& min, const D3DXVECTOR3& max);

   bool isCollidingWith(const D3DXVECTOR3& point) const;

   bool isCollidingWith(const AABoundingBox& rhs) const;

   bool isCollidingWith(const Ray& ray) const;

   AABoundingBox operator+(const AABoundingBox& rhs) const;

   D3DXVECTOR3 findIntersectionRemovalVector(const AABoundingBox& colidor) const;

private:
   inline bool areRangesOverlapping(float min1, float max1, float min2, float max2) const;
   
};

//-----------------------------------------------------------------------------
// (helper struct)
// the structure describes a sphere bouyyng an object
//-----------------------------------------------------------------------------
struct BoundingSphere
{
   D3DXVECTOR3 origin;
   float radius;

   BoundingSphere();
   BoundingSphere(const D3DXVECTOR3& origin, float radius);

   bool isCollidingWith(const D3DXVECTOR3& point) const;

   bool isCollidingWith(const BoundingSphere& rhs) const;

   D3DXVECTOR3 findIntersectionRemovalVector(const BoundingSphere& colidor) const;
};

//-----------------------------------------------------------------------------
// (helper struct)
// the structure describes a ray
//-----------------------------------------------------------------------------
struct Ray
{
   D3DXVECTOR3 origin;
   D3DXVECTOR3 direction;

   Ray(const D3DXVECTOR3& _origin, const D3DXVECTOR3& _direction)
      : origin(_origin), direction(_direction)
   {
   }

   bool isIntersecting(const D3DXPLANE& plane, D3DXVECTOR3& intersectionPt) const;
};
