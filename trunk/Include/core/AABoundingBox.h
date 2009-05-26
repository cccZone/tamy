#pragma once

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * the structure describes an axis aligned bounding box
 */
struct AABoundingBox
{
   D3DXVECTOR3 min;
   D3DXVECTOR3 max;

   AABoundingBox();
   AABoundingBox(const D3DXVECTOR3& min, const D3DXVECTOR3& max);

   AABoundingBox operator+(const AABoundingBox& rhs) const; 
};

///////////////////////////////////////////////////////////////////////////////
