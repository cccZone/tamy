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

   Plane plane(0, 0, 1, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 1, 0, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(-1, 0, 0, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, -2);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, 1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, 3);
   CPPUNIT_ASSERT(0 < vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, -3);
   CPPUNIT_ASSERT(0 > vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane));

   AABoundingBox zeroVol(Vector(0, 0, 0), Vector(0, 0, 0));
   plane = Plane(0, 0, 1, 0);
   CPPUNIT_ASSERT_EQUAL(0.f,  zeroVol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_COPLANAR, zeroVol.classifyAgainsPlane(plane));
}

///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, BoundingSphere)
{
   BoundingSphere vol(Vector(0, 0, 0), 1);

   Plane plane(0, 0, 1, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 1, 0, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(1, 0, 0, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, -1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, 1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, 2);
   CPPUNIT_ASSERT(0 < vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, -2);
   CPPUNIT_ASSERT(0 > vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane));

   BoundingSphere zeroVol(Vector(0, 0, 0), 0);
   plane = Plane(0, 0, 1, 0);
   CPPUNIT_ASSERT_EQUAL(0.f,  zeroVol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_COPLANAR, zeroVol.classifyAgainsPlane(plane));
}

///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, PointVolume)
{
   PointVolume vol(Vector(0, 0, 0));

   Plane plane(0, 0, 1, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_COPLANAR, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, 1);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) > 0);
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, -1);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) < 0);
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane));
}

///////////////////////////////////////////////////////////////////////////////

TEST(PlaneClassification, Triangle)
{
   Triangle vol(Vector(-1, 0, 1), Vector(1, 0, 1), Vector(-1, 0, -1));

   // oy
   Plane plane(0, 1, 0, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_COPLANAR, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 1, 0, 1);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) > 0);
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 1, 0, -1);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) < 0);
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane));

   // ox
   plane = Plane(1, 0, 0, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(1, 0, 0, -1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(1, 0, 0, 1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(1, 0, 0, 2);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) > 0);
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane));

   plane = Plane(1, 0, 0, -2);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) < 0);
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane));

   // oz
   plane = Plane(0, 0, 1, 0);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, -1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, 1);
   CPPUNIT_ASSERT_EQUAL(0.f, vol.distanceToPlane(plane));
   CPPUNIT_ASSERT_EQUAL(PPC_SPANNING, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, 2);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) > 0);
   CPPUNIT_ASSERT_EQUAL(PPC_FRONT, vol.classifyAgainsPlane(plane));

   plane = Plane(0, 0, 1, -2);
   CPPUNIT_ASSERT(vol.distanceToPlane(plane) < 0);
   CPPUNIT_ASSERT_EQUAL(PPC_BACK, vol.classifyAgainsPlane(plane));
}

///////////////////////////////////////////////////////////////////////////////
