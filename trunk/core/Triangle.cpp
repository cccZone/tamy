#include "core\Triangle.h"


///////////////////////////////////////////////////////////////////////////////

Triangle::Triangle(const Triangle& rhs)
{
   v[0] = rhs.v[0];
   v[1] = rhs.v[1];
   v[2] = rhs.v[2];

   e[0] = rhs.e[0];
   e[1] = rhs.e[1];
   e[2] = rhs.e[2];

   en[0] = rhs.en[0];
   en[1] = rhs.en[1];
   en[2] = rhs.en[2];
}

///////////////////////////////////////////////////////////////////////////////

Triangle::Triangle(const D3DXVECTOR3& pt1,
                   const D3DXVECTOR3& pt2,
                   const D3DXVECTOR3& pt3)
{
   v[0] = pt1;
   v[1] = pt2;
   v[2] = pt3;
 
   // calculate the edges
   e[0] = v[1] - v[0];
   e[1] = v[2] - v[1];
   e[2] = v[0] - v[2];

   // calculate the edge normals
   D3DXVECTOR3 tmpPerpVec;
   D3DXVec3Cross(&tmpPerpVec, &e[1], &e[0]);
   D3DXVec3Cross(&en[0], &tmpPerpVec, &e[0]);
   D3DXVec3Normalize(&en[0], &en[0]);

   D3DXVec3Cross(&tmpPerpVec, &e[2], &e[1]);
   D3DXVec3Cross(&en[1], &tmpPerpVec, &e[1]);
   D3DXVec3Normalize(&en[1], &en[1]);

   D3DXVec3Cross(&tmpPerpVec, &e[0], &e[2]);
   D3DXVec3Cross(&en[2], &tmpPerpVec, &e[2]);
   D3DXVec3Normalize(&en[2], &en[2]);
}

///////////////////////////////////////////////////////////////////////////////
