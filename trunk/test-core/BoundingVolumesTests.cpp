#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\CollisionTests.h"
#include "core\AABoundingBox.h"
#include "core\Ray.h"
#include "core\BoundingSphere.h"
#include "core\Frustum.h"
#include "core\Triangle.h"
#include "core\MatrixWriter.h"


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

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(D3DXVECTOR3(0, 2, 0), D3DXVECTOR3(0, -1, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(D3DXVECTOR3(0, 2, 0), D3DXVECTOR3(0, 1, 0))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(D3DXVECTOR3(0, -2, 0), D3DXVECTOR3(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(D3DXVECTOR3(0, -2, 0), D3DXVECTOR3(0, -1, 0))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(D3DXVECTOR3(-2, 0, 0), D3DXVECTOR3(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(D3DXVECTOR3(-2, 0, 0), D3DXVECTOR3(-1, 0, 0))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(D3DXVECTOR3(0, 0, -2), D3DXVECTOR3(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(D3DXVECTOR3(0, 0, -2), D3DXVECTOR3(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(D3DXVECTOR3(2, 2, 2), D3DXVECTOR3(-1, -1, -1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(D3DXVECTOR3(2, 2, 2), D3DXVECTOR3(1, 1, 1))));

   // ray originating inside the box
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1))));
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
   frustrum.planes[0]  = D3DXPLANE(         0,          0,         -1, 1.01f);
   frustrum.planes[1]  = D3DXPLANE(         0,          0,          1, -5002.28f);
   frustrum.planes[2]  = D3DXPLANE(-0.707107f,          0, -0.707107f, 0);
   frustrum.planes[3]  = D3DXPLANE( 0.707107f,          0, -0.707107f, 0);
   frustrum.planes[4]  = D3DXPLANE(         0,  0.707107f, -0.707107f, 0);
   frustrum.planes[5]  = D3DXPLANE(         0, -0.707107f, -0.707107f, 0);

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

TEST(BoundingSphere, intersects_Ray)
{
   BoundingSphere sphere(D3DXVECTOR3(0, 0, 0), 3);

   // various tests for rays going directly through or away from the sphere's center
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(D3DXVECTOR3(0, 0, -10), D3DXVECTOR3(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(D3DXVECTOR3(0, 0, -10), D3DXVECTOR3(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(D3DXVECTOR3(0, 0, 10), D3DXVECTOR3(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(D3DXVECTOR3(0, 0, 10), D3DXVECTOR3(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(D3DXVECTOR3(-10, 0, 0), D3DXVECTOR3(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(D3DXVECTOR3(-10, 0, 0), D3DXVECTOR3(-1, 0, 0))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(D3DXVECTOR3(10, 0, 0), D3DXVECTOR3(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(D3DXVECTOR3(10, 0, 0), D3DXVECTOR3(-1, 0, 0))));

   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(D3DXVECTOR3(0, -10, 0), D3DXVECTOR3(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(D3DXVECTOR3(0, -10, 0), D3DXVECTOR3(0, -1, 0))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(D3DXVECTOR3(0, 10, 0), D3DXVECTOR3(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(D3DXVECTOR3(0, 10, 0), D3DXVECTOR3(0, -1, 0))));

   // tests testing rays going through the sphere, but not directly through it's center
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(D3DXVECTOR3(2, 0, -10), D3DXVECTOR3(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(D3DXVECTOR3(2, 0, -10), D3DXVECTOR3(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(D3DXVECTOR3(-2, 0, 10), D3DXVECTOR3(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(D3DXVECTOR3(-2, 0, 10), D3DXVECTOR3(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(D3DXVECTOR3(-10, 0, 2), D3DXVECTOR3(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(D3DXVECTOR3(-10, 0, 2), D3DXVECTOR3(-1, 0, 0))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(D3DXVECTOR3(10, 0, -2), D3DXVECTOR3(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(D3DXVECTOR3(10, 0, -2), D3DXVECTOR3(-1, 0, 0))));

   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(D3DXVECTOR3(2, -10, 0), D3DXVECTOR3(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(D3DXVECTOR3(2, -10, 0), D3DXVECTOR3(0, -1, 0))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(D3DXVECTOR3(0, 10, 2), D3DXVECTOR3(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(D3DXVECTOR3(0, 10, 2), D3DXVECTOR3(0, -1, 0))));

   // tests testing rays that originate inside the sphere
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, -1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -1, 0))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(D3DXVECTOR3(1, 1, 0), D3DXVECTOR3(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(D3DXVECTOR3(0, 1, -1), D3DXVECTOR3(0, 0, -1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(D3DXVECTOR3(-1, 0, 1), D3DXVECTOR3(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(-1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(D3DXVECTOR3(0, 2, 0.5f), D3DXVECTOR3(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(D3DXVECTOR3(1.5f, 0, 1.5f), D3DXVECTOR3(0, -1, 0))));
}

///////////////////////////////////////////////////////////////////////////////

TEST(Ray, intersects_Plane)
{
   D3DXPLANE plane(0, 0, -1, 0);
   D3DXVECTOR3 intersectionPt;

   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(D3DXVECTOR3(0, 0, -1), D3DXVECTOR3(0, 0, 1)), plane, intersectionPt));
   CPPUNIT_ASSERT_EQUAL(D3DXVECTOR3(0, 0, 0), intersectionPt);
   
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(0, 0, -1), D3DXVECTOR3(0, 0, -1)), plane, intersectionPt));

   // ray coming in from behind the plane - in that case even if it crosses it, 
   // there will be no intersection - the plane points elsewhere
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, 0, -1)), plane, intersectionPt));
   CPPUNIT_ASSERT_EQUAL(D3DXVECTOR3(0, 0, 0), intersectionPt);

   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, 0, 1)), plane, intersectionPt));

   // another case
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(D3DXVECTOR3(0, 0, 0.5f), D3DXVECTOR3(0, 0, 1)), 
                                             D3DXPLANE(0, -1, 0, 0), 
                                             intersectionPt));
   CPPUNIT_ASSERT_EQUAL(D3DXVECTOR3(0, 0, 0.5f), intersectionPt);

   // another case
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(-2, 0, 0), D3DXVECTOR3(0, 0, 1)), 
                                             D3DXPLANE(-1, 0, 0, -1), 
                                             intersectionPt));
}

///////////////////////////////////////////////////////////////////////////////

TEST(Triangle, intersects_Ray)
{
   Triangle tri(D3DXVECTOR3(-3, 1, 10), D3DXVECTOR3(3, 1, 10), D3DXVECTOR3(-3, -1, 10));
   
   // test against triangle interior
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0, 1)), tri));

   // test against triangle edges
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(D3DXVECTOR3(-3, 0, 0), D3DXVECTOR3(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1)), tri));

   // test against triangle vertices
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(D3DXVECTOR3(-3, 1, 0), D3DXVECTOR3(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(D3DXVECTOR3(3, 1, 0), D3DXVECTOR3(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(D3DXVECTOR3(-3, -1, 0), D3DXVECTOR3(0, 0, 1)), tri));

   // test against triangle exterior
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(-3.1, 1.1, 0), D3DXVECTOR3(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(3.1, 1.1, 0), D3DXVECTOR3(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(-3.1, -1.1, 0), D3DXVECTOR3(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(-3.1, 0, 0), D3DXVECTOR3(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(0, 1.1, 0), D3DXVECTOR3(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(0, -0.1, 0), D3DXVECTOR3(0, 0, 1)), tri));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(0, 0, 8), D3DXVECTOR3(0, 1, 0)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(0, 0, 12), D3DXVECTOR3(0, 1, 0)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(0, 0, 8), D3DXVECTOR3(1, 0, 0)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(D3DXVECTOR3(0, 0, 12), D3DXVECTOR3(1, 0, 0)), tri));

}

///////////////////////////////////////////////////////////////////////////////
