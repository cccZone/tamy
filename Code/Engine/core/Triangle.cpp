#include "core\Triangle.h"
#include "core\CollisionTests.h"


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

BoundingVolume* Triangle::operator*(const D3DXMATRIX& mtx) const
{
   D3DXVECTOR3 newV[3];
   D3DXVec3TransformCoord(&newV[0], &v[0], &mtx);
   D3DXVec3TransformCoord(&newV[1], &v[1], &mtx);
   D3DXVec3TransformCoord(&newV[2], &v[2], &mtx);

   return new Triangle(newV[0], newV[1], newV[2]);
}

///////////////////////////////////////////////////////////////////////////////

void Triangle::operator*=(const D3DXMATRIX& mtx)
{
   D3DXVec3TransformCoord(&v[0], &v[0], &mtx);
   D3DXVec3TransformCoord(&v[1], &v[1], &mtx);
   D3DXVec3TransformCoord(&v[2], &v[2], &mtx);

   D3DXVec3TransformCoord(&e[0], &e[0], &mtx);
   D3DXVec3TransformCoord(&e[1], &e[1], &mtx);
   D3DXVec3TransformCoord(&e[2], &e[2], &mtx);

   D3DXVec3TransformNormal(&en[0], &en[0], &mtx);
   D3DXVec3TransformNormal(&en[1], &en[1], &mtx);
   D3DXVec3TransformNormal(&en[2], &en[2], &mtx);
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 Triangle::splitEdge(float percentage,
                                unsigned int startVtxIdx, 
                                unsigned int endVtxIdx) const
{
   D3DXVECTOR3 v1 = v[startVtxIdx];
   D3DXVECTOR3 edge = v[endVtxIdx] - v1;
   return v1 + (edge * percentage);
}

///////////////////////////////////////////////////////////////////////////////

PlaneClassification Triangle::classifyAgainsPlane(const D3DXPLANE& plane) const
{
    float dist1 = D3DXPlaneDotCoord(&plane, &v[0]);
    float dist2 = D3DXPlaneDotCoord(&plane, &v[1]);
    float dist3 = D3DXPlaneDotCoord(&plane, &v[2]);

    if ((dist1 < 0) && (dist2 < 0) && (dist3 < 0))
    {
        return PPC_BACK;
    }
    else if ((dist1 > 0) && (dist2 > 0) && (dist3 > 0))
    {
        return PPC_FRONT;
    }
    else if ((dist1 == 0) && (dist2 == 0) && (dist3 == 0))
    {
        return PPC_COPLANAR;
    }
    else
    {
        return PPC_SPANNING;
    }
}

///////////////////////////////////////////////////////////////////////////////

float Triangle::distanceToPlane(const D3DXPLANE& plane) const
{
   int frontCount = 0;
   int backCount = 0;
   for (unsigned int i = 0; i < 3; ++i)
   {
      D3DXVECTOR3 planeNormal(plane.a, plane.b, plane.c);
      float dist = D3DXVec3Dot(&v[i], &planeNormal) + plane.d;

      if (dist > 0.0001f)
      {
         ++frontCount;
      }
      else if (dist < -0.0001f)
      {
         ++backCount;
      }
   }

   if (frontCount == 3) {return 1;}
   else if (backCount == 3) {return -1;}
   else {return 0;}
}

///////////////////////////////////////////////////////////////////////////////

void Triangle::split(const D3DXPLANE& splitPlane, 
                     Array<Triangle*>& frontSplit, 
                     Array<Triangle*>& backSplit)
{
   m_splitter.split(*this, splitPlane, frontSplit, backSplit);
}

///////////////////////////////////////////////////////////////////////////////

bool Triangle::testCollision(const PointVolume& point) const
{
   ASSERT(false, "Triangle::testCollision(const PointVolume&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Triangle::testCollision(const AABoundingBox& rhs) const
{
   ASSERT(false, "Triangle::testCollision(const AABoundingBox&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Triangle::testCollision(const BoundingSphere& rhs) const
{
   ASSERT(false, "Triangle::testCollision(const BoundingSphere&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Triangle::testCollision(const Frustum& rhs) const
{
   ASSERT(false, "Triangle::testCollision(const Frustum&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Triangle::testCollision(const Ray& rhs) const
{
   return ::testCollision(rhs, *this);
}

///////////////////////////////////////////////////////////////////////////////

bool Triangle::testCollision(const Triangle& rhs) const
{
   ASSERT(false, "Triangle::testCollision(const Triangle&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////
