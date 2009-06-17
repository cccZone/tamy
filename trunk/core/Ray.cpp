#include "core\Ray.h"


///////////////////////////////////////////////////////////////////////////////

Ray Ray::operator*(const D3DXMATRIX& mtx) const
{
   Ray newRay;

   D3DXVec3TransformCoord(&newRay.origin, &origin, &mtx);
   D3DXVec3TransformNormal(&newRay.direction, &direction, &mtx);

   return newRay;
}

///////////////////////////////////////////////////////////////////////////////
