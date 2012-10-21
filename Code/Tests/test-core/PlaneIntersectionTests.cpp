#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\CollisionTests.h"
#include "core\AABoundingBox.h"
#include "core\Ray.h"
#include "core\BoundingSphere.h"
#include "core\PointVolume.h"
#include "core\Triangle.h"
#include "core\Plane.h"
#include "core-TestFramework\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, AABoundingBox)
{
   AABoundingBox vol(Vector(-1, -1, -1), Vector(2, 2, 2));

   Plane plane;
   plane.set( Quad_1000, Float_0 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0100, Float_0 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_Neg_1000, Float_0 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_Minus2 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_1 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_3 );
   CPPUNIT_ASSERT(0 < vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, FastFloat::fromFloat( -3.0f ) );
   CPPUNIT_ASSERT(0 > vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane) );

   AABoundingBox zeroVol(Vector(0, 0, 0), Vector(0, 0, 0));
   plane.set( Quad_0010, Float_0 );
   CPPUNIT_ASSERT_EQUAL(0.f,  zeroVol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_COPLANAR, zeroVol.classifyAgainsPlane(plane) );
}

///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, BoundingSphere)
{
   BoundingSphere vol(Vector(0, 0, 0), 1);

   Plane plane;
   plane.set( Quad_0010, Float_0 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0100, Float_0 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_1000, Float_0 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_Minus1 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_1 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_2 );
   CPPUNIT_ASSERT(0 < vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_Minus2 );
   CPPUNIT_ASSERT(0 > vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane) );

   BoundingSphere zeroVol(Vector(0, 0, 0), 0);
   plane.set( Quad_0010, Float_0 );
   CPPUNIT_ASSERT_EQUAL(0.f,  zeroVol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_COPLANAR, zeroVol.classifyAgainsPlane(plane) );
}

///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, PointVolume)
{
   PointVolume vol(Vector(0, 0, 0));

   Plane plane;
   plane.set( Quad_0010, Float_0 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_COPLANAR, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_1 );
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) > Float_0);
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_Minus1 );
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) < Float_0);
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane) );
}

///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, Triangle)
{
   Triangle vol(Vector(-1, 0, 1), Vector(1, 0, 1), Vector(-1, 0, -1));

   // oy
   Plane plane;
   plane.set( Quad_0100, Float_0 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_COPLANAR, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0100, Float_1 );
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) > Float_0);
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0100, Float_Minus1 );
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) < Float_0);
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane) );

   // ox
   plane.set( Quad_1000, Float_0 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_1000, Float_Minus1 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_1000, Float_1 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_1000, Float_2 );
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) > Float_0 );
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_1000, Float_Minus2 );
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) < Float_0 );
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane) );

   // oz
   plane.set( Quad_0010, Float_0 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_Minus1 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_1 );
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane).getFloat() );
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_2 );
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) > Float_0 );
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane) );

   plane.set( Quad_0010, Float_Minus2 );
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) < Float_0 );
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane) );
}

///////////////////////////////////////////////////////////////////////////////
