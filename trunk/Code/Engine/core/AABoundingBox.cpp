#include "core\AABoundingBox.h"
#include "core\Assert.h"
#include "core\CollisionTests.h"
#include "core\PointVolume.h"
#include "core\Matrix.h"
#include "core\Plane.h"


///////////////////////////////////////////////////////////////////////////////

AABoundingBox::AABoundingBox()
{
   min.set( FLT_MAX, FLT_MAX, FLT_MAX );
   max.set( -FLT_MAX, -FLT_MAX, -FLT_MAX );
}

///////////////////////////////////////////////////////////////////////////////

AABoundingBox::AABoundingBox( const Vector& _min, const Vector& _max )
{
   min = _min;
   max = _max;
}

///////////////////////////////////////////////////////////////////////////////

void AABoundingBox::add( const AABoundingBox& otherBox, AABoundingBox& unionBox ) const
{
   unionBox.min.x = this->min.x < otherBox.min.x ? this->min.x : otherBox.min.x;
   unionBox.min.y = this->min.y < otherBox.min.y ? this->min.y : otherBox.min.y;
   unionBox.min.z = this->min.z < otherBox.min.z ? this->min.z : otherBox.min.z;

   unionBox.max.x = this->max.x > otherBox.max.x ? this->max.x : otherBox.max.x;
   unionBox.max.y = this->max.y > otherBox.max.y ? this->max.y : otherBox.max.y;
   unionBox.max.z = this->max.z > otherBox.max.z ? this->max.z : otherBox.max.z;
}

///////////////////////////////////////////////////////////////////////////////

void AABoundingBox::reset()
{
   min.set( FLT_MAX, FLT_MAX, FLT_MAX );
   max.set( -FLT_MAX, -FLT_MAX, -FLT_MAX );
}

///////////////////////////////////////////////////////////////////////////////

BoundingVolume* AABoundingBox::clone() const
{
   return new AABoundingBox( min, max );
}

///////////////////////////////////////////////////////////////////////////////

void AABoundingBox::transform( const Matrix& mtx, BoundingVolume& transformedVolume ) const
{
   // verify that the volume is an AABoundingBox
   ASSERT( dynamic_cast< AABoundingBox* >( &transformedVolume ) != NULL );
   AABoundingBox& transformedBox = static_cast< AABoundingBox& >( transformedVolume );

   float av, bv;
   transformedBox.min = transformedBox.max = Vector( mtx.m[3][0], mtx.m[3][1], mtx.m[3][2] );

   for ( int i = 0; i < 3; ++i)
   {
      for ( int j = 0; j < 3; ++j )
      {
         av = mtx.m[i][j] * min[j];
         bv = mtx.m[i][j] * max[j];
         if (av < bv)
         {
            transformedBox.min[i] += av;
            transformedBox.max[i] += bv;
         } 
         else 
         {
            transformedBox.min[i] += bv;
            transformedBox.max[i] += av;
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void AABoundingBox::calculateBoundingBox( AABoundingBox& outBoundingBox ) const
{
   outBoundingBox.min = min;
   outBoundingBox.max = max;
}

///////////////////////////////////////////////////////////////////////////////

float AABoundingBox::distanceToPlane(const Plane& plane) const
{
   Vector planeNormal(plane.a, plane.b, plane.c);

   Vector nearPoint, farPoint;

   if (planeNormal.x > 0.0f) {farPoint.x = max.x; nearPoint.x = min.x;}
   else {farPoint.x = min.x; nearPoint.x = max.x;}
   if (planeNormal.y > 0.0f) {farPoint.y = max.y; nearPoint.y = min.y;}
   else {farPoint.y = min.y; nearPoint.y = max.y;}
   if (planeNormal.z > 0.0f) {farPoint.z = max.z; nearPoint.z = min.z;}
   else {farPoint.z = min.z; nearPoint.z = max.z;}

   if ( plane.dotCoord( nearPoint ) > 0.0f)
   {
      // the box is in front
      return 1;
   }
   if ( plane.dotCoord( farPoint ) >= 0.0f)
   {
      return 0;
   }
   else
   {
      // the box is behind
      return -1;
   }
}

///////////////////////////////////////////////////////////////////////////////

bool AABoundingBox::testCollision(const PointVolume& point) const
{
   return ::testCollision(*this, point.point);
}

///////////////////////////////////////////////////////////////////////////////

bool AABoundingBox::testCollision(const AABoundingBox& rhs) const
{
   return ::testCollision(*this, rhs);
}

///////////////////////////////////////////////////////////////////////////////

bool AABoundingBox::testCollision(const BoundingSphere& rhs) const
{
   return ::testCollision(*this, rhs);
}

///////////////////////////////////////////////////////////////////////////////

bool AABoundingBox::testCollision(const Frustum& rhs) const
{
   return ::testCollision(*this, rhs);
}

///////////////////////////////////////////////////////////////////////////////

bool AABoundingBox::testCollision(const Ray& rhs) const
{
   return ::testCollision(*this, rhs);
}

///////////////////////////////////////////////////////////////////////////////

bool AABoundingBox::testCollision(const Triangle& rhs) const
{
   ASSERT_MSG(false, "AABoundingBox::testCollision(const Triangle&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool AABoundingBox::hasVolume() const
{
   return (bool)(min != max);
}

///////////////////////////////////////////////////////////////////////////////

void AABoundingBox::include(const Vector& pt)
{
   for (char i = 0; i < 3; ++i)
   {
      if ( pt[i] < min[i] ) 
      {
         min[i] = pt[i];
      }
      if ( pt[i] > max[i] ) 
      {
         max[i] = pt[i];
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
