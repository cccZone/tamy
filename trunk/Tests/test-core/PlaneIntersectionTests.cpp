#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\CollisionTests.h"
#include "core\AABoundingBox.h"
#include "core\Ray.h"
#include "core\BoundingSphere.h"
#include "core\PointVolume.h"
#include "core\Triangle.h"
#include "core\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, AABoundingBox)
{
   AABoundingBox vol(D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(2, 2, 2));

   D3DXPLANE plane(0, 0, 1, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 1, 0, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(-1, 0, 0, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, -2);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, 1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, 3);
   CPPUNIT_ASSERT(0 < vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, -3);
   CPPUNIT_ASSERT(0 > vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane));

   AABoundingBox zeroVol(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
   plane = D3DXPLANE(0, 0, 1, 0);
   CPPUNIT_ASSERT_EQUAL(0.f,  zeroVol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_COPLANAR, zeroVol.classifyAgainsPlane(plane));
}

///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, BoundingSphere)
{
   BoundingSphere vol(D3DXVECTOR3(0, 0, 0), 1);

   D3DXPLANE plane(0, 0, 1, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 1, 0, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(1, 0, 0, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, -1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, 1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, 2);
   CPPUNIT_ASSERT(0 < vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, -2);
   CPPUNIT_ASSERT(0 > vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane));

   BoundingSphere zeroVol(D3DXVECTOR3(0, 0, 0), 0);
   plane = D3DXPLANE(0, 0, 1, 0);
   CPPUNIT_ASSERT_EQUAL(0.f,  zeroVol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_COPLANAR, zeroVol.classifyAgainsPlane(plane));
}

///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, PointVolume)
{
   PointVolume vol(D3DXVECTOR3(0, 0, 0));

   D3DXPLANE plane(0, 0, 1, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_COPLANAR, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, 1);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) > 0);
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, -1);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) < 0);
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane));
}

///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, Triangle)
{
   Triangle vol(D3DXVECTOR3(-1, 0, 1), D3DXVECTOR3(1, 0, 1), D3DXVECTOR3(-1, 0, -1));

   // oy
   D3DXPLANE plane(0, 1, 0, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_COPLANAR, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 1, 0, 1);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) > 0);
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 1, 0, -1);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) < 0);
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane));

   // ox
   plane = D3DXPLANE(1, 0, 0, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(1, 0, 0, -1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(1, 0, 0, 1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(1, 0, 0, 2);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) > 0);
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(1, 0, 0, -2);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) < 0);
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane));

   // oz
   plane = D3DXPLANE(0, 0, 1, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, -1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, 1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, 2);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) > 0);
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane));

   plane = D3DXPLANE(0, 0, 1, -2);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) < 0);
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane));
}

///////////////////////////////////////////////////////////////////////////////
