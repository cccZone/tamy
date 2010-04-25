#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\Triangle.h"
#include "core\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, notCuttingPlaneUsed)
{
   Triangle vol(D3DXVECTOR3(-1, 0, 1), D3DXVECTOR3(1, 0, 1), D3DXVECTOR3(-1, 0, -1));

   // plane that the triangle is behind
   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(D3DXPLANE(0, 0, 1, -2), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, backSplit.size());
   CPPUNIT_ASSERT(NULL != backSplit[0]);

   COMPARE_VEC(D3DXVECTOR3(-1, 0,  1), backSplit[0]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 1, 0,  1), backSplit[0]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3(-1, 0, -1), backSplit[0]->vertex(2));

   delete backSplit[0];
   backSplit.clear();

   // plane that the triangle is in front of
   vol.split(D3DXPLANE(0, 0, 1, 2), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, backSplit.size());
   CPPUNIT_ASSERT(NULL != frontSplit[0]);

   COMPARE_VEC(D3DXVECTOR3(-1, 0,  1), frontSplit[0]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 1, 0,  1), frontSplit[0]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3(-1, 0, -1), frontSplit[0]->vertex(2));

   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, cuttingTriangleThroughTwoEdges)
{
   Triangle vol(D3DXVECTOR3(-1, 0, 1), D3DXVECTOR3(1, 0, 1), D3DXVECTOR3(-1, 0, -1));

   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(D3DXPLANE(1, 0, 0, 0), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, backSplit.size());

   COMPARE_VEC(D3DXVECTOR3( 0, 0,  1), frontSplit[0]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 1, 0,  1), frontSplit[0]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3( 0, 0,  0), frontSplit[0]->vertex(2));

   COMPARE_VEC(D3DXVECTOR3(-1, 0,  1), backSplit[0]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 0, 0,  1), backSplit[0]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3( 0, 0,  0), backSplit[0]->vertex(2));

   COMPARE_VEC(D3DXVECTOR3(-1, 0,  1), backSplit[1]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 0, 0,  0), backSplit[1]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3(-1, 0, -1), backSplit[1]->vertex(2));

   delete backSplit[0];
   delete backSplit[1];
   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, cuttingTrianlgeThorughEdgeAndVertex)
{
   Triangle vol(D3DXVECTOR3(-1, 0, 1), D3DXVECTOR3(1, 0, 1), D3DXVECTOR3(0, 0, -1));

   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(D3DXPLANE(1, 0, 0, 0), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, backSplit.size());

   COMPARE_VEC(D3DXVECTOR3(-1, 0,  1), backSplit[0]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 0, 0,  1), backSplit[0]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3( 0, 0, -1), backSplit[0]->vertex(2));

   COMPARE_VEC(D3DXVECTOR3( 0, 0,  1), frontSplit[0]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 1, 0,  1), frontSplit[0]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3( 0, 0, -1), frontSplit[0]->vertex(2));

   delete backSplit[0];
   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, planeIsOnOneOfTheEdges)
{
   Triangle vol(D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(1, 0, 1), D3DXVECTOR3(0, 0, 0));

   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(D3DXPLANE(1, 0, 0, 0), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, backSplit.size());

   COMPARE_VEC(D3DXVECTOR3(0, 0, 1), frontSplit[0]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3(1, 0, 1), frontSplit[0]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3(0, 0, 0), frontSplit[0]->vertex(2));

   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, planeIsOnOneOfTheVertices)
{
   Triangle vol(D3DXVECTOR3(-1, 0, 1), D3DXVECTOR3(1, 0, 1), D3DXVECTOR3(0, 0, 0));

   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(D3DXPLANE(0, 0, 1, 0), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, backSplit.size());

   COMPARE_VEC(D3DXVECTOR3(-1, 0, 1), frontSplit[0]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 1, 0, 1), frontSplit[0]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3( 0, 0, 0), frontSplit[0]->vertex(2));

   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, nonStandardTriangle)
{
   Triangle vol(D3DXVECTOR3(-1.5f, 0, 1), 
                D3DXVECTOR3( 2.5f, 0, 2.5f), 
                D3DXVECTOR3( 0.5f, 0,-2.5f));

   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(D3DXPLANE(1, 0, 0, -1.5f), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, backSplit.size());

   COMPARE_VEC(D3DXVECTOR3(1.5f, 0, 2.125f),  frontSplit[0]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3(2.5f, 0, 2.5f),    frontSplit[0]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3(1.5f, 0, 0),       frontSplit[0]->vertex(2));

   COMPARE_VEC(D3DXVECTOR3(-1.5f, 0, 1),      backSplit[0]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 1.5f, 0, 2.125f), backSplit[0]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3( 1.5f, 0, 0),      backSplit[0]->vertex(2));

   COMPARE_VEC(D3DXVECTOR3(-1.5f, 0, 1),      backSplit[1]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 1.5f, 0, 0),      backSplit[1]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3( 0.5f, 0,-2.5f),   backSplit[1]->vertex(2));

   delete backSplit[0];
   delete backSplit[1];
   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////

TEST(TriangleSplitter, bugTriangle)
{
   Triangle vol(D3DXVECTOR3( 11, -29,  2), 
                D3DXVECTOR3( 11, -29, -2), 
                D3DXVECTOR3(  0, -29, -2));

   Array<Triangle*> frontSplit;
   Array<Triangle*> backSplit;
   vol.split(D3DXPLANE(0, 0, 1, 0), frontSplit, backSplit);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, frontSplit.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, backSplit.size());

   COMPARE_VEC(D3DXVECTOR3(  11, -29,  2),  frontSplit[0]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3(  11, -29,  0),  frontSplit[0]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3(5.5f, -29,  0),  frontSplit[0]->vertex(2));

   COMPARE_VEC(D3DXVECTOR3(  11, -29,  0),  backSplit[0]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3(  11, -29, -2),  backSplit[0]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3(   0, -29, -2),  backSplit[0]->vertex(2));

   COMPARE_VEC(D3DXVECTOR3(  11, -29,  0),  backSplit[1]->vertex(0));
   COMPARE_VEC(D3DXVECTOR3(   0, -29, -2),  backSplit[1]->vertex(1));
   COMPARE_VEC(D3DXVECTOR3(5.5f, -29,  0),  backSplit[1]->vertex(2));

   delete backSplit[0];
   delete backSplit[1];
   delete frontSplit[0];
}

///////////////////////////////////////////////////////////////////////////////
