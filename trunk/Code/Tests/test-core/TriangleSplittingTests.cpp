#include "core-TestFramework\TestFramework.h"
#include "core\Triangle.h"
#include "core-TestFramework\MatrixWriter.h"
#include "core\Plane.h"


///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, notCuttingPlaneUsed)
{
   Triangle vol(Vector(-1, 0, 1), Vector(1, 0, 1), Vector(-1, 0, -1));

   // plane that the triangle is behind
   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(Plane(0, 0, 1, -2), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, backSplit.size());
   CPPUNIT_ASSERT(NULL != backSplit[0]);

   COMPARE_VEC(Vector(-1, 0,  1), backSplit[0]->vertex(0));
   COMPARE_VEC(Vector( 1, 0,  1), backSplit[0]->vertex(1));
   COMPARE_VEC(Vector(-1, 0, -1), backSplit[0]->vertex(2));

   delete backSplit[0];
   backSplit.clear();

   // plane that the triangle is in front of
   vol.split(Plane(0, 0, 1, 2), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, backSplit.size());
   CPPUNIT_ASSERT(NULL != frontSplit[0]);

   COMPARE_VEC(Vector(-1, 0,  1), frontSplit[0]->vertex(0));
   COMPARE_VEC(Vector( 1, 0,  1), frontSplit[0]->vertex(1));
   COMPARE_VEC(Vector(-1, 0, -1), frontSplit[0]->vertex(2));

   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, cuttingTriangleThroughTwoEdges)
{
   Triangle vol(Vector(-1, 0, 1), Vector(1, 0, 1), Vector(-1, 0, -1));

   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(Plane(1, 0, 0, 0), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, backSplit.size());

   COMPARE_VEC(Vector( 0, 0,  1), frontSplit[0]->vertex(0));
   COMPARE_VEC(Vector( 1, 0,  1), frontSplit[0]->vertex(1));
   COMPARE_VEC(Vector( 0, 0,  0), frontSplit[0]->vertex(2));

   COMPARE_VEC(Vector(-1, 0,  1), backSplit[0]->vertex(0));
   COMPARE_VEC(Vector( 0, 0,  1), backSplit[0]->vertex(1));
   COMPARE_VEC(Vector( 0, 0,  0), backSplit[0]->vertex(2));

   COMPARE_VEC(Vector(-1, 0,  1), backSplit[1]->vertex(0));
   COMPARE_VEC(Vector( 0, 0,  0), backSplit[1]->vertex(1));
   COMPARE_VEC(Vector(-1, 0, -1), backSplit[1]->vertex(2));

   delete backSplit[0];
   delete backSplit[1];
   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, cuttingTrianlgeThorughEdgeAndVertex)
{
   Triangle vol(Vector(-1, 0, 1), Vector(1, 0, 1), Vector(0, 0, -1));

   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(Plane(1, 0, 0, 0), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, backSplit.size());

   COMPARE_VEC(Vector(-1, 0,  1), backSplit[0]->vertex(0));
   COMPARE_VEC(Vector( 0, 0,  1), backSplit[0]->vertex(1));
   COMPARE_VEC(Vector( 0, 0, -1), backSplit[0]->vertex(2));

   COMPARE_VEC(Vector( 0, 0,  1), frontSplit[0]->vertex(0));
   COMPARE_VEC(Vector( 1, 0,  1), frontSplit[0]->vertex(1));
   COMPARE_VEC(Vector( 0, 0, -1), frontSplit[0]->vertex(2));

   delete backSplit[0];
   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, planeIsOnOneOfTheEdges)
{
   Triangle vol(Vector(0, 0, 1), Vector(1, 0, 1), Vector(0, 0, 0));

   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(Plane(1, 0, 0, 0), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, backSplit.size());

   COMPARE_VEC(Vector(0, 0, 1), frontSplit[0]->vertex(0));
   COMPARE_VEC(Vector(1, 0, 1), frontSplit[0]->vertex(1));
   COMPARE_VEC(Vector(0, 0, 0), frontSplit[0]->vertex(2));

   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, planeIsOnOneOfTheVertices)
{
   Triangle vol(Vector(-1, 0, 1), Vector(1, 0, 1), Vector(0, 0, 0));

   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(Plane(0, 0, 1, 0), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, backSplit.size());

   COMPARE_VEC(Vector(-1, 0, 1), frontSplit[0]->vertex(0));
   COMPARE_VEC(Vector( 1, 0, 1), frontSplit[0]->vertex(1));
   COMPARE_VEC(Vector( 0, 0, 0), frontSplit[0]->vertex(2));

   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, nonStandardTriangle)
{
   Triangle vol(Vector(-1.5f, 0, 1), 
                Vector( 2.5f, 0, 2.5f), 
                Vector( 0.5f, 0,-2.5f));

   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(Plane(1, 0, 0, -1.5f), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, backSplit.size());

   COMPARE_VEC(Vector(1.5f, 0, 2.125f),  frontSplit[0]->vertex(0));
   COMPARE_VEC(Vector(2.5f, 0, 2.5f),    frontSplit[0]->vertex(1));
   COMPARE_VEC(Vector(1.5f, 0, 0),       frontSplit[0]->vertex(2));

   COMPARE_VEC(Vector(-1.5f, 0, 1),      backSplit[0]->vertex(0));
   COMPARE_VEC(Vector( 1.5f, 0, 2.125f), backSplit[0]->vertex(1));
   COMPARE_VEC(Vector( 1.5f, 0, 0),      backSplit[0]->vertex(2));

   COMPARE_VEC(Vector(-1.5f, 0, 1),      backSplit[1]->vertex(0));
   COMPARE_VEC(Vector( 1.5f, 0, 0),      backSplit[1]->vertex(1));
   COMPARE_VEC(Vector( 0.5f, 0,-2.5f),   backSplit[1]->vertex(2));

   delete backSplit[0];
   delete backSplit[1];
   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, bugTriangle)
{
   Triangle vol(Vector( 11, -29,  2), 
                Vector( 11, -29, -2), 
                Vector(  0, -29, -2));

   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(Plane(0, 0, 1, 0), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, backSplit.size());

   COMPARE_VEC(Vector(  11, -29,  2),  frontSplit[0]->vertex(0));
   COMPARE_VEC(Vector(  11, -29,  0),  frontSplit[0]->vertex(1));
   COMPARE_VEC(Vector(5.5f, -29,  0),  frontSplit[0]->vertex(2));

   COMPARE_VEC(Vector(  11, -29,  0),  backSplit[0]->vertex(0));
   COMPARE_VEC(Vector(  11, -29, -2),  backSplit[0]->vertex(1));
   COMPARE_VEC(Vector(   0, -29, -2),  backSplit[0]->vertex(2));

   COMPARE_VEC(Vector(  11, -29,  0),  backSplit[1]->vertex(0));
   COMPARE_VEC(Vector(   0, -29, -2),  backSplit[1]->vertex(1));
   COMPARE_VEC(Vector(5.5f, -29,  0),  backSplit[1]->vertex(2));

   delete backSplit[0];
   delete backSplit[1];
   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////
