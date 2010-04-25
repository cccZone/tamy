#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\CollisionTests.h"
#include "core\AABoundingBox.h"
#include "core\Ray.h"
#include "core\BoundingSphere.h"
#include "core\PointVolume.h"
#include "core\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(RayDistance, PointVolume)
{
   Ray ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 0, 0));

   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0f, rayToPointDistance(ray, D3DXVECTOR3(0, 0, 0)), 1e-3);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(20.0f, rayToPointDistance(ray, D3DXVECTOR3(20, 0, 0)), 1e-3);
   CPPUNIT_ASSERT_EQUAL(FLT_MAX, rayToPointDistance(ray, D3DXVECTOR3(-1, 0, 0)));
   CPPUNIT_ASSERT_EQUAL(FLT_MAX, rayToPointDistance(ray, D3DXVECTOR3(0, 1, 0)));
   CPPUNIT_ASSERT_EQUAL(FLT_MAX, rayToPointDistance(ray, D3DXVECTOR3(0, 0, 1)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(RayDistance, BoundingSphere)
{
   Ray ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 0, 0));

   BoundingSphere bs(D3DXVECTOR3(0, 0, 0), 1);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0f, rayToBSDistance(ray, bs), 1e-3);

   bs.origin = D3DXVECTOR3(10, 0, 0);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0f, rayToBSDistance(ray, bs), 1e-3);

   bs.origin = D3DXVECTOR3(-10, 0, 0);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(FLT_MAX, rayToBSDistance(ray, bs), 1e-3);

   bs.origin = D3DXVECTOR3(10, 0.5f, 0);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(9.134f, rayToBSDistance(ray, bs), 1e-3);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RayDistance, AABoundingBox)
{
   AABoundingBox aabb(D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(1, 1, 1));

   Ray ray1(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 0, 0));
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0f, rayToAABBDistance(ray1, aabb), 1e-3);

   Ray ray2(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-1, 0, 0));
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0f, rayToAABBDistance(ray2, aabb), 1e-3);

   Ray ray3(D3DXVECTOR3(-10, 0, 0), D3DXVECTOR3(1, 0, 0));
   CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0f, rayToAABBDistance(ray3, aabb), 1e-3);

   Ray ray4(D3DXVECTOR3(-10, 0, 0), D3DXVECTOR3(-1, 0, 0));
   CPPUNIT_ASSERT_DOUBLES_EQUAL(FLT_MAX, rayToAABBDistance(ray4, aabb), 1e-3);

   Ray ray5(D3DXVECTOR3(0, 10, 0), D3DXVECTOR3(0, -1, 0));
   CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0f, rayToAABBDistance(ray5, aabb), 1e-3);

   Ray ray6(D3DXVECTOR3(10, 10, 10), D3DXVECTOR3(-1, -1, -1));
   CPPUNIT_ASSERT_DOUBLES_EQUAL(15.588f, rayToAABBDistance(ray6, aabb), 1e-3);

   Ray ray7(D3DXVECTOR3(-10, 5, 0), D3DXVECTOR3(1, 0, 0));
   CPPUNIT_ASSERT_DOUBLES_EQUAL(FLT_MAX, rayToAABBDistance(ray7, aabb), 1e-3);
}

///////////////////////////////////////////////////////////////////////////////
