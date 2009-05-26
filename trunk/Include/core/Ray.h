#pragma once

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * the structure describes a ray
 */
struct Ray
{
   D3DXVECTOR3 origin;
   D3DXVECTOR3 direction;

   Ray(const D3DXVECTOR3& _origin, const D3DXVECTOR3& _direction)
      : origin(_origin), direction(_direction)
   {}
};

///////////////////////////////////////////////////////////////////////////////
