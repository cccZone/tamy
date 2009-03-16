#include "core\BoundingVolume.h"


///////////////////////////////////////////////////////////////////////////////

AABoundingBox::AABoundingBox()
{
   min = D3DXVECTOR3(99999999.f, 99999999.f, 99999999.f);
   max = D3DXVECTOR3(-99999999.f, -99999999.f, -99999999.f);
}

///////////////////////////////////////////////////////////////////////////////

AABoundingBox::AABoundingBox(const D3DXVECTOR3& _min, const D3DXVECTOR3& _max)
{
   min = _min;
   max = _max;
}

///////////////////////////////////////////////////////////////////////////////

AABoundingBox AABoundingBox::operator+(const AABoundingBox& rhs) const
{
   AABoundingBox newBB;

   newBB.min.x = this->min.x < rhs.min.x ? this->min.x : rhs.min.x;
   newBB.min.y = this->min.y < rhs.min.y ? this->min.y : rhs.min.y;
   newBB.min.z = this->min.z < rhs.min.z ? this->min.z : rhs.min.z;

   newBB.max.x = this->max.x > rhs.max.x ? this->max.x : rhs.max.x;
   newBB.max.y = this->max.y > rhs.max.y ? this->max.y : rhs.max.y;
   newBB.max.z = this->max.z > rhs.max.z ? this->max.z : rhs.max.z;

   return newBB;
}

///////////////////////////////////////////////////////////////////////////////

bool AABoundingBox::isCollidingWith(const D3DXVECTOR3& point) const
{
   return areRangesOverlapping(min.x, max.x, point.x, point.x) && 
      areRangesOverlapping(min.y, max.y, point.y, point.y) && 
      areRangesOverlapping(min.z, max.z, point.z, point.z);
}

///////////////////////////////////////////////////////////////////////////////

bool AABoundingBox::isCollidingWith(const AABoundingBox& rhs) const
{
   UINT collidingPlanes = 0;
   if (areRangesOverlapping(this->min.x, this->max.x, rhs.min.x, rhs.max.x))
   {
      collidingPlanes++;
   }

   if (areRangesOverlapping(this->min.y, this->max.y, rhs.min.y, rhs.max.y))
   {
      collidingPlanes++;
   }

   if (areRangesOverlapping(this->min.z, this->max.z, rhs.min.z, rhs.max.z))
   {
      collidingPlanes++;
   }

   return (collidingPlanes == 3);
}

///////////////////////////////////////////////////////////////////////////////

bool AABoundingBox::areRangesOverlapping(float min1, float max1, float min2, float max2) const
{
   return !((max1 < min2) || (max2 < min1));
}

///////////////////////////////////////////////////////////////////////////////

bool AABoundingBox::isCollidingWith(const Ray& ray) const
{
   D3DXPLANE plane;
   D3DXVECTOR3 intersectionPt(0, 0, 0);

   plane = D3DXPLANE(0, 0, -1, -min.z);
   if ((ray.isIntersecting(plane, intersectionPt) == true) &&
       areRangesOverlapping(min.x, max.x, intersectionPt.x, intersectionPt.x) &&
       areRangesOverlapping(min.y, max.y, intersectionPt.y, intersectionPt.y))
   {
      return true;
   }

   plane = D3DXPLANE(0, 0, 1, max.z);
   if ((ray.isIntersecting(plane, intersectionPt) == true) &&
       areRangesOverlapping(min.x, max.x, intersectionPt.x, intersectionPt.x) &&
       areRangesOverlapping(min.y, max.y, intersectionPt.y, intersectionPt.y))
   {
      return true;
   }

   plane = D3DXPLANE(0, -1, 0, -min.y);
   if ((ray.isIntersecting(plane, intersectionPt) == true) &&
       areRangesOverlapping(min.x, max.x, intersectionPt.x, intersectionPt.x) &&
       areRangesOverlapping(min.z, max.z, intersectionPt.z, intersectionPt.z))
   {
      return true;
   }

   plane = D3DXPLANE(0, 1, 0, max.y);
   if ((ray.isIntersecting(plane, intersectionPt) == true) &&
       areRangesOverlapping(min.x, max.x, intersectionPt.x, intersectionPt.x) &&
       areRangesOverlapping(min.z, max.z, intersectionPt.z, intersectionPt.z))
   {
      return true;
   }

   plane = D3DXPLANE(-1, 0, 0, -min.x);
   if ((ray.isIntersecting(plane, intersectionPt) == true) &&
       areRangesOverlapping(min.z, max.z, intersectionPt.z, intersectionPt.z) &&
       areRangesOverlapping(min.y, max.y, intersectionPt.y, intersectionPt.y))
   {
      return true;
   }

   plane = D3DXPLANE(1, 0, 0, max.x);
   if ((ray.isIntersecting(plane, intersectionPt) == true) &&
       areRangesOverlapping(min.z, max.z, intersectionPt.z, intersectionPt.z) &&
       areRangesOverlapping(min.y, max.y, intersectionPt.y, intersectionPt.y))
   {
      return true;
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 AABoundingBox::findIntersectionRemovalVector(const AABoundingBox& colidor) const
{
   D3DXVECTOR3 vec(0, 0, 0);

   if (areRangesOverlapping(min.x, max.x, colidor.max.x, colidor.max.x))
   {
      vec.x = min.x - colidor.max.x;
   }
   else if (areRangesOverlapping(min.x, max.x, colidor.min.x, colidor.min.x))
   {
      vec.x = max.x - colidor.min.x;
   }

   if (areRangesOverlapping(min.z, max.z, colidor.max.z, colidor.max.z))
   {
      vec.z = min.z - colidor.max.z;
   }
   else if (areRangesOverlapping(min.z, max.z, colidor.min.z, colidor.min.z))
   {
      vec.z = max.z - colidor.min.z;
   }

   return vec;
}

///////////////////////////////////////////////////////////////////////////////

bool Ray::isIntersecting(const D3DXPLANE& plane, D3DXVECTOR3& intersectionPt) const
{
   D3DXVECTOR3 pn(plane.a, plane.b, plane.c);

   float Vd = D3DXVec3Dot(&pn, &direction);
   if (Vd >= 0) return false;

   float V0 = -D3DXVec3Dot(&pn, &origin) + plane.d;
   float t = V0 / Vd;
   bool isIntersecting = (V0 / Vd >= 0);

   if (isIntersecting)
   {
      intersectionPt.x = origin.x + direction.x * t;
      intersectionPt.y = origin.y + direction.y * t;
      intersectionPt.z = origin.z + direction.z * t;
   }

   return isIntersecting;
}

///////////////////////////////////////////////////////////////////////////////

BoundingSphere::BoundingSphere()
      : origin(0, 0, 0),
      radius(0)
{
}

///////////////////////////////////////////////////////////////////////////////

BoundingSphere::BoundingSphere(const D3DXVECTOR3& _origin, float _radius)
      : origin(_origin),
      radius(_radius)
{
}

///////////////////////////////////////////////////////////////////////////////

bool BoundingSphere::isCollidingWith(const D3DXVECTOR3& point) const
{
   D3DXVECTOR3 to = this->origin - point;
   return (D3DXVec3Length(&to) < this->radius*this->radius);
}

///////////////////////////////////////////////////////////////////////////////

bool BoundingSphere::isCollidingWith(const BoundingSphere& rhs) const
{
   D3DXVECTOR3 to = this->origin - rhs.origin;
   float totalRadius  = this->radius + rhs.radius;

   return ((this != &rhs) && (D3DXVec3Length(&to) < totalRadius*totalRadius));
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 BoundingSphere::findIntersectionRemovalVector(const BoundingSphere& colidor) const
{
   D3DXVECTOR3 toColidor = colidor.origin - this->origin;
   float distFromEachOther = D3DXVec3Length(&toColidor);

   float amountOfOverLap = colidor.radius + this->radius - distFromEachOther;
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
