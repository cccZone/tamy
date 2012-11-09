#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\CollisionTests.h"
#include "core\AABoundingBox.h"
#include "core\Ray.h"
#include "core\BoundingSphere.h"
#include "core\Frustum.h"
#include "core\Triangle.h"
#include "core-TestFramework\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(AABoundingBox, intersects_AABoundingBox)
{
   AABoundingBox centralBB(Vector(-1, -1, -1), Vector(1, 1, 1));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(Vector(-2, -2, -2), Vector(-1.5, -1.5, -1.5))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(Vector(1.5, 1.5, 1.5), Vector(2, 2, 2))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(Vector(-2, -1, -1), Vector(-1.5, 1, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(Vector(-1, -2, -1), Vector(1, -1.5, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(Vector(-1, -1, -2), Vector(1, 1, -1.5))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(Vector(1.5, -1, -1), Vector(2, 1, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(Vector(-1, 1.5, -1), Vector(1, 2, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, AABoundingBox(Vector(-1, -1, 1.5), Vector(1, 1, 2))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(Vector(-1, -1, -1), Vector(1, 1, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(Vector(-1, -1, -1), Vector(0, 1, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(Vector(-1, -1, -1), Vector(1, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(Vector(-1, -1, -1), Vector(1, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(Vector(0, -1, -1), Vector(1, 1, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(Vector(-1, 0, -1), Vector(1, 1, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, AABoundingBox(Vector(-1, -1, 0), Vector(1, 1, 1))));
}

///////////////////////////////////////////////////////////////////////////////

TEST(AABoundingBox, intersects_Ray)
{
   AABoundingBox centralBB(Vector(-1, -1, -1), Vector(1, 1, 1));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(Vector(-2, 0, 0), Vector(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(Vector(2, 0, 0), Vector(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(Vector(0, 2, 0), Vector(0, -1, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(Vector(0, 2, 0), Vector(0, 1, 0))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(Vector(0, -2, 0), Vector(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(Vector(0, -2, 0), Vector(0, -1, 0))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(Vector(-2, 0, 0), Vector(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(Vector(-2, 0, 0), Vector(-1, 0, 0))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(Vector(0, 0, -2), Vector(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(Vector(0, 0, -2), Vector(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(Vector(2, 2, 2), Vector(-1, -1, -1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Ray(Vector(2, 2, 2), Vector(1, 1, 1))));

   // ray originating inside the box
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Ray(Vector(0, 0, 0), Vector(0, 0, 1))));
}

///////////////////////////////////////////////////////////////////////////////

TEST(AABoundingBox, intersects_Point)
{
   AABoundingBox centralBB(Vector(-1, -1, -1), Vector(1, 1, 1));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Vector(-2, 0, 0)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Vector(2, 0, 0)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Vector(0, -2, 0)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Vector(0, 2, 0)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Vector(0, 0, -2)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, Vector(0, 0, 2)));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Vector(0, 0, 0)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Vector(1, 0, 0.5)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, Vector(0, -0.5, 0.5)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(AABoundingBox, intersects_BoundingSphere)
{
   AABoundingBox centralBB(Vector(-1, -1, -1), Vector(1, 1, 1));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(Vector(0, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(Vector(1, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(Vector(0, 1, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(Vector(0, 0, 1), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(Vector(-1, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(Vector(0, -1, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(centralBB, BoundingSphere(Vector(0, 0, -1), 1)));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, BoundingSphere(Vector(2.5, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, BoundingSphere(Vector(0, 2.5, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, BoundingSphere(Vector(0, 0, 2.5), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, BoundingSphere(Vector(-2.5, 0, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, BoundingSphere(Vector(0, -2.5, 0), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(centralBB, BoundingSphere(Vector(0, 0, -2.5), 1)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(BoundingSphere, intersects_Frustrum)
{
   Frustum frustrum;
   const FastFloat ff_707 = FastFloat::fromFloat( 0.707107f );
   const FastFloat ff_neg_707 = FastFloat::fromFloat( -0.707107f );

   frustrum.planes[0].set( Float_0,    Float_0,    Float_1,       FastFloat::fromFloat( -1.01f ) );
   frustrum.planes[1].set( Float_0,    Float_0,    Float_Minus1,  FastFloat::fromFloat( 5002.28f ) );
   frustrum.planes[2].set( ff_707,     Float_0,    ff_707,        Float_0 );
   frustrum.planes[3].set( ff_neg_707, Float_0,    ff_707,        Float_0 );
   frustrum.planes[4].set( Float_0,    ff_neg_707, ff_707,        Float_0 );
   frustrum.planes[5].set( Float_0,    ff_707,     ff_707,        Float_0 );

   // completely inside
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 0, 100), 1)));

   // approaching left plane
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(-9, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(-10, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(-11, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(Vector(-12, 0, 10), 1)));

   // approaching right plane
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(9, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(10, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(11, 0, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(Vector(12, 0, 10), 1)));

   // approaching upper plane
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 9, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 10, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 11, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(Vector(0, 12, 10), 1)));

   // approaching lower plane
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, -9, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, -10, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, -11, 10), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(Vector(0, -12, 10), 1)));

   // approaching front plane
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 0, 5001), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 0, 5002), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 0, 5003), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(Vector(0, 0, 5004), 1)));

   // approaching near plane
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 0, 2), 1)));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(frustrum, BoundingSphere(Vector(0, 0, 1), 1)));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(frustrum, BoundingSphere(Vector(0, 0, 0), 1)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(BoundingSphere, intersects_Ray)
{
   BoundingSphere sphere(Vector(0, 0, 0), 3);

   // various tests for rays going directly through or away from the sphere's center
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(Vector(0, 0, -10), Vector(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(Vector(0, 0, -10), Vector(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(Vector(0, 0, 10), Vector(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(Vector(0, 0, 10), Vector(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(Vector(-10, 0, 0), Vector(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(Vector(-10, 0, 0), Vector(-1, 0, 0))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(Vector(10, 0, 0), Vector(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(Vector(10, 0, 0), Vector(-1, 0, 0))));

   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(Vector(0, -10, 0), Vector(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(Vector(0, -10, 0), Vector(0, -1, 0))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(Vector(0, 10, 0), Vector(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(Vector(0, 10, 0), Vector(0, -1, 0))));

   // tests testing rays going through the sphere, but not directly through it's center
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(Vector(2, 0, -10), Vector(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(Vector(2, 0, -10), Vector(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(Vector(-2, 0, 10), Vector(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(Vector(-2, 0, 10), Vector(0, 0, -1))));

   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(Vector(-10, 0, 2), Vector(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(Vector(-10, 0, 2), Vector(-1, 0, 0))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(Vector(10, 0, -2), Vector(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(Vector(10, 0, -2), Vector(-1, 0, 0))));

   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(Vector(2, -10, 0), Vector(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(Vector(2, -10, 0), Vector(0, -1, 0))));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(sphere, Ray(Vector(0, 10, 2), Vector(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(true,  testCollision(sphere, Ray(Vector(0, 10, 2), Vector(0, -1, 0))));

   // tests testing rays that originate inside the sphere
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(Vector(0, 0, 0), Vector(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(Vector(0, 0, 0), Vector(0, 0, -1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(Vector(0, 0, 0), Vector(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(Vector(0, 0, 0), Vector(-1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(Vector(0, 0, 0), Vector(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(Vector(0, 0, 0), Vector(0, -1, 0))));

   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(Vector(1, 1, 0), Vector(0, 0, 1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(Vector(0, 1, -1), Vector(0, 0, -1))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(Vector(-1, 0, 1), Vector(1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(Vector(0, 1, 0), Vector(-1, 0, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(Vector(0, 2, 0.5f), Vector(0, 1, 0))));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(sphere, Ray(Vector(1.5f, 0, 1.5f), Vector(0, -1, 0))));
}

///////////////////////////////////////////////////////////////////////////////

TEST(Ray, intersects_Plane)
{
   Plane plane;
   plane.set( Quad_Neg_0010, Float_0 );
   Vector intersectionPt;

   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(Vector(0, 0, -1), Vector(0, 0, 1)), plane, intersectionPt));
   COMPARE_VEC(Vector(0, 0, 0), intersectionPt);
   
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(Vector(0, 0, -1), Vector(0, 0, -1)), plane, intersectionPt));

   // ray coming in from behind the plane - in that case even if it crosses it, 
   // there will be no intersection - the plane points elsewhere
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(Vector(0, 0, 1), Vector(0, 0, -1)), plane, intersectionPt));
   COMPARE_VEC(Vector(0, 0, 0), intersectionPt);

   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(Vector(0, 0, 1), Vector(0, 0, 1)), plane, intersectionPt));

   // another case
   Plane testPlane;
   testPlane.set( Quad_Neg_0100, Float_0 );
   CPPUNIT_ASSERT_EQUAL( true, testCollision( Ray( Vector(0, 0, 0.5f), Vector(0, 0, 1) ), testPlane, intersectionPt ) );
   COMPARE_VEC(Vector(0, 0, 0.5f), intersectionPt);

   // another case
   testPlane.set( Quad_Neg_1000, Float_Minus1 );
   CPPUNIT_ASSERT_EQUAL( false, testCollision( Ray( Vector(-2, 0, 0), Vector(0, 0, 1)), testPlane, intersectionPt ) );
}

///////////////////////////////////////////////////////////////////////////////

TEST(Triangle, intersects_Ray)
{
   Triangle tri(Vector(-3, 1, 10), Vector(3, 1, 10), Vector(-3, -1, 10));
   
   // test against triangle interior
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(Vector(-1, 0, 0), Vector(0, 0, 1)), tri));

   // test against triangle edges
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(Vector(-3, 0, 0), Vector(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(Vector(0, 1, 0), Vector(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(Vector(0, 0, 0), Vector(0, 0, 1)), tri));

   // test against triangle vertices
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(Vector(-3, 1, 0), Vector(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(Vector(3, 1, 0), Vector(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(true, testCollision(Ray(Vector(-3, -1, 0), Vector(0, 0, 1)), tri));

   // test against triangle exterior
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(Vector(-3.1f, 1.1f, 0), Vector(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(Vector(3.1f, 1.1f, 0), Vector(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(Vector(-3.1f, -1.1f, 0), Vector(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(Vector(-3.1f, 0, 0), Vector(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(Vector(0, 1.1f, 0), Vector(0, 0, 1)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(Vector(0, -0.1f, 0), Vector(0, 0, 1)), tri));

   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(Vector(0, 0, 8), Vector(0, 1, 0)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(Vector(0, 0, 12), Vector(0, 1, 0)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(Vector(0, 0, 8), Vector(1, 0, 0)), tri));
   CPPUNIT_ASSERT_EQUAL(false, testCollision(Ray(Vector(0, 0, 12), Vector(1, 0, 0)), tri));
}

///////////////////////////////////////////////////////////////////////////////
