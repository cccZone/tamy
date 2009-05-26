#pragma once

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * The structure describes a sphere bounding an object
 */
struct BoundingSphere
{
   D3DXVECTOR3 origin;
   float radius;

   BoundingSphere();
   BoundingSphere(const D3DXVECTOR3& origin, float radius);
};

///////////////////////////////////////////////////////////////////////////////
