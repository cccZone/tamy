#include "core\AABoundingBox.h"
#include "core\Assert.h"
#include "core\CollisionTests.h"


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

BoundingVolume* AABoundingBox::operator*(const D3DXMATRIX& mtx) const
{
   AABoundingBox* newBB = new AABoundingBox();

   D3DXVec3TransformCoord(&newBB->min, &min, &mtx);
   D3DXVec3TransformCoord(&newBB->max, &max, &mtx);

   return newBB;
}

///////////////////////////////////////////////////////////////////////////////

bool AABoundingBox::testCollision(const D3DXVECTOR3& point) const
{
   return ::testCollision(*this, point);
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
   ASSERT(false, "AABoundingBox::testCollision(const Triangle&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////
