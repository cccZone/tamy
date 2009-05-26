#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\CollisionTests.h"
#include "core\AABoundingBox.h"
#include "core\Ray.h"
#include "core\BoundingSphere.h"
#include "core\Frustum.h"


///////////////////////////////////////////////////////////////////////////////

TEST(AABoundingBox, intersects_AABoundingBox)
{
   AABoundingBox centralBB(D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(1, 1, 1));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(-2, -2, -2), D3DXVECTOR3(-1.5, -1.5, -1.5))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(1.5, 1.5, 1.5), D3DXVECTOR3(2, 2, 2))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(-2, -1, -1), D3DXVECTOR3(-1.5, 1, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(-1, -2, -1), D3DXVECTOR3(1, -1.5, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(-1, -1, -2), D3DXVECTOR3(1, 1, -1.5))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(1.5, -1, -1), D3DXVECTOR3(2, 1, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(-1, 1.5, -1), D3DXVECTOR3(1, 2, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(-1, -1, 1.5), D3DXVECTOR3(1, 1, 2))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(1, 1, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(0, 1, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(1, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(1, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(0, -1, -1), D3DXVECTOR3(1, 1, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(-1, 0, -1), D3DXVECTOR3(1, 1, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(D3DXVECTOR3(-1, -1, 0), D3DXVECTOR3(1, 1, 1))));
}

///////////////////////////////////////////////////////////////////////////////

TEST(AABoundingBox, intersects_Ray)
{
   AABoundingBox centralBB(D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(1, 1, 1));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(D3DXVECTOR3(-2, 0, 0), D3DXVECTOR3(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(D3DXVECTOR3(2, 0, 0), D3DXVECTOR3(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(D3DXVECTOR3(0, -2, 0), D3DXVECTOR3(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(D3DXVECTOR3(0, -2, 0), D3DXVECTOR3(0, -1, 0))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(D3DXVECTOR3(-2, 0, 0), D3DXVECTOR3(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(D3DXVECTOR3(-2, 0, 0), D3DXVECTOR3(-1, 0, 0))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(D3DXVECTOR3(0, 0, -2), D3DXVECTOR3(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(D3DXVECTOR3(0, 0, -2), D3DXVECTOR3(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(D3DXVECTOR3(2, 2, 2), D3DXVECTOR3(-1, -1, -1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(D3DXVECTOR3(2, 2, 2), D3DXVECTOR3(1, 1, 1))));
}

///////////////////////////////////////////////////////////////////////////////

TEST(AABoundingBox, intersects_Point)
{
   AABoundingBox centralBB(D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(1, 1, 1));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, D3DXVECTOR3(-2, 0, 0)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, D3DXVECTOR3(2, 0, 0)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, D3DXVECTOR3(0, -2, 0)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, D3DXVECTOR3(0, 2, 0)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, D3DXVECTOR3(0, 0, -2)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, D3DXVECTOR3(0, 0, 2)));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, D3DXVECTOR3(0, 0, 0)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, D3DXVECTOR3(1, 0, 0.5)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, D3DXVECTOR3(0, -0.5, 0.5)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(AABoundingBox, intersects_BoundingSphere)
{
   AABoundingBox centralBB(D3DXVECTOR3(-1, -1, -1), D3DXVECTOR3(1, 1, 1));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(0, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(1, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(0, 1, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(0, 0, 1), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(-1, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(0, -1, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(0, 0, -1), 1)));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(2.5, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(0, 2.5, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(0, 0, 2.5), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(-2.5, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(0, -2.5, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, BoundingSphere(D3DXVECTOR3(0, 0, -2.5), 1)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(BoundingSphere, intersects_Frustrum)
{
   Frustum frustrum;
   frustrum.nearPlane  = D3DXPLANE(         0,          0,         -1, 1.01f);
   frustrum.farPlane   = D3DXPLANE(         0,          0,          1, -5002.28f);
   frustrum.leftPlane  = D3DXPLANE(-0.707107f,          0, -0.707107f, 0);
   frustrum.rightPlane = D3DXPLANE( 0.707107f,          0, -0.707107f, 0);
   frustrum.upperPlane = D3DXPLANE(         0,  0.707107f, -0.707107f, 0);
   frustrum.lowerPlane = D3DXPLANE(         0, -0.707107f, -0.707107f, 0);

   // completely inside
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 100), 1)));

   // approaching left plane
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(-9, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(-10, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(-11, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(-12, 0, 10), 1)));

   // approaching right plane
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(9, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(10, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(11, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(12, 0, 10), 1)));

   // approaching upper plane
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 9, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 10, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 11, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 12, 10), 1)));

   // approaching lower plane
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, -9, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, -10, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, -11, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, -12, 10), 1)));

   // approaching front plane
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 5001), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 5002), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 5003), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 5004), 1)));

   // approaching near plane
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 2), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 1), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(D3DXVECTOR3(0, 0, 0), 1)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(Ray, intersects_Plane)
{
   D3DXPLANE plane(0, 0, -1, 0);
   D3DXVECTOR3 intersectionPt;

   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(D3DXVECTOR3(0, 0, -1), D3DXVECTOR3(0, 0, 1)), plane, intersectionPt));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, 0, -1)), plane, intersectionPt));
}

///////////////////////////////////////////////////////////////////////////////

TEST(BoundingSphere_Frustrum_intersection, bug)
{
   Frustum frustrum;

   frustrum.nearPlane  = D3DXPLANE(0.643853, -0.051671, -0.763403, 1.01);
   frustrum.farPlane   = D3DXPLANE(-0.000186903, 1, 0.00022161, -1.45241);
   frustrum.leftPlane  = D3DXPLANE(-0.193365, -0.0385902, -0.980368, -0);
   frustrum.rightPlane = D3DXPLANE(0.999354, -0.0260927, 0.0247237, -0);
   frustrum.upperPlane = D3DXPLANE(0.345464, 0.839005, -0.420387, -0);
   frustrum.lowerPlane = D3DXPLANE(0.298389, -0.890676, -0.343016, -0);

   BoundingSphere sphere(D3DXVECTOR3(-9.9418507, 5.1547689, 17.066504), 3.7042301);

   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, sphere));
}

///////////////////////////////////////////////////////////////////////////////
