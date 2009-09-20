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

   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, 0)));
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 1, 0, 0)));
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(-1, 0, 0, 0)));
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, -2)));
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, 1)));

   CPPUNIT_ASSERT(0 < vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, 3)));
   CPPUNIT_ASSERT(0 > vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, -3)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, BoundingSphere)
{
   BoundingSphere vol(D3DXVECTOR3(0, 0, 0), 1);

   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, 0)));
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 1, 0, 0)));
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(1, 0, 0, 0)));
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, -1)));
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, 1)));

   CPPUNIT_ASSERT(0 < vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, 2)));
   CPPUNIT_ASSERT(0 > vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, -2)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, PointVolume)
{
   PointVolume vol(D3DXVECTOR3(0, 0, 0));

   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, 0)));
   CPPUNIT_ASSERT(vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, 1)) > 0);
   CPPUNIT_ASSERT(vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, -1)) < 0);
}

///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, Triangle)
{
   Triangle vol(D3DXVECTOR3(-1, 0, 1), D3DXVECTOR3(1, 0, 1), D3DXVECTOR3(-1, 0, -1));

   // oy
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 1, 0, 0)));
   CPPUNIT_ASSERT(vol.classifyAgainstPlane(D3DXPLANE(0, 1, 0, 1)) > 0);
   CPPUNIT_ASSERT(vol.classifyAgainstPlane(D3DXPLANE(0, 1, 0, -1)) < 0);

   // ox
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(1, 0, 0, 0)));
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(1, 0, 0, -1)));
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(1, 0, 0, 1)));
   CPPUNIT_ASSERT(vol.classifyAgainstPlane(D3DXPLANE(1, 0, 0, 2)) > 0);
   CPPUNIT_ASSERT(vol.classifyAgainstPlane(D3DXPLANE(1, 0, 0, -2)) < 0);

   // oz
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, 0)));
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, -1)));
   CPPUNIT_ASSERT_EQUAL(0.f, vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, 1)));
   CPPUNIT_ASSERT(vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, 2)) > 0);
   CPPUNIT_ASSERT(vol.classifyAgainstPlane(D3DXPLANE(0, 0, 1, -2)) < 0);
}

///////////////////////////////////////////////////////////////////////////////
