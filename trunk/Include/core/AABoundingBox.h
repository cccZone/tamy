#pragma once

#include "core\BoundingVolume.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * the structure describes an axis aligned bounding box
 */
struct AABoundingBox : public BoundingVolume
{
   D3DXVECTOR3 min;
   D3DXVECTOR3 max;

   AABoundingBox();
   AABoundingBox(const D3DXVECTOR3& min, const D3DXVECTOR3& max);

   AABoundingBox operator+(const AABoundingBox& rhs) const; 
   BoundingVolume* operator*(const D3DXMATRIX& mtx) const;

   bool testCollision(const D3DXVECTOR3& point) const;
   bool testCollision(const AABoundingBox& rhs) const;
   bool testCollision(const BoundingSphere& rhs) const;
   bool testCollision(const Frustum& rhs) const;
   bool testCollision(const Ray& rhs) const;
   bool testCollision(const Triangle& rhs) const;
   bool testCollision(const BoundingVolume& rhs) const {return rhs.testCollision(*this);}
};

///////////////////////////////////////////////////////////////////////////////