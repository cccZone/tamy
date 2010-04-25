#include "core-TestFramework\TestFramework.h"
#include "core\Array.h"
#include "core\PointVolume.h"
#include "core\TriangleSplitter.h"
#include "core\MatrixWriter.h"
#include "core\BoundingVolume.h"
#include "core\BSPNodeTree.h"
#include <d3dx9.h>

///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

   struct TriangleMock : public SplittableTriangle<D3DXVECTOR3>
   {
      D3DXVECTOR3 v[3];

      TriangleMock(const D3DXVECTOR3& v1,
         const D3DXVECTOR3& v2,
         const D3DXVECTOR3& v3)
      {
         v[0] = v1;
         v[1] = v2;
         v[2] = v3;
      }

      D3DXPLANE getPlane() const
      {
         D3DXPLANE plane;
         D3DXPlaneFromPoints(&plane, &v[0], &v[2], &v[1]);
         return plane;
      }

      PlaneClassification classifyAgainst(const D3DXPLANE& plane)
      {
         float dist1 = D3DXPlaneDotCoord(&plane, &v[0]);
         float dist2 = D3DXPlaneDotCoord(&plane, &v[1]);
         float dist3 = D3DXPlaneDotCoord(&plane, &v[2]);

         if ((dist1 < 0) && (dist2 < 0) && (dist3 < 0))
         {
            return PPC_BACK;
         }
         else if ((dist1 > 0) && (dist2 > 0) && (dist3 > 0))
         {
            return PPC_FRONT;
         }
         else if ((dist1 == 0) && (dist2 == 0) && (dist3 == 0))
         {
            return PPC_COPLANAR;
         }
         else
         {
            return PPC_SPANNING;
         }
      }

      const D3DXVECTOR3& vertex(unsigned int vtxIdx) const {return v[vtxIdx];}

      const D3DXVECTOR3& vertexPos(unsigned int vtxIdx) const {return v[vtxIdx];}

      D3DXVECTOR3 splitEdge(float percentage, 
         unsigned int startVtxIdx, 
         unsigned int endVtxIdx) const
      {
         D3DXVECTOR3 v1 = v[startVtxIdx];
         D3DXVECTOR3 edge = v[endVtxIdx] - v1;
         return v1 + (edge * percentage);
      }

      void split(const D3DXPLANE& plane, 
         Array<TriangleMock*>& frontSplit, 
         Array<TriangleMock*>& backSplit)
      {
         TriangleSplitter<D3DXVECTOR3, TriangleMock> splitter;
         splitter.split(*this, plane, frontSplit, backSplit);
      }

      bool operator==(const TriangleMock& rhs) const
      {
         bool match = true;
         for (int i = 0; i < 3; ++i)
         {
            float dist = D3DXVec3Length(&(v[i] - rhs.v[i]));
            if (dist > 1e-3) 
            {
               match = false;
               break;
            }
         }

         return match;
      }

      friend std::ostream& operator<<(std::ostream &os, const TriangleMock& t);
   };

   std::ostream& operator<<(std::ostream &os, const TriangleMock& t)
   {
      os << "{" << t.v[0] << " " << t.v[1] << " " << t.v[2] << "}";
      return os;
   }

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(BSPNodeTree, singleTriangle)
{
   Array<TriangleMock*> geometry;
   TriangleMock* triangle = new TriangleMock(D3DXVECTOR3(-1, 1, 0), D3DXVECTOR3(1, 1, 0), D3DXVECTOR3(-1, 0, 0));
   geometry.push_back(triangle);

   BSPNodeTree<TriangleMock> tree(geometry);

   // query the tree for triangles from various positions
   Array<TriangleMock*> queryResult;

   tree.query(D3DXVECTOR3(0, 0, -1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, queryResult.size());
   queryResult.clear();

   tree.query(D3DXVECTOR3(0, 0, 1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, queryResult.size());
   queryResult.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST(BSPNodeTree, multipleTrianglesNonSplitting)
{
   Array<TriangleMock*> geometry;
   TriangleMock* triangle1 = new TriangleMock(D3DXVECTOR3(-1, 1, 1), D3DXVECTOR3(1, 1, 1), D3DXVECTOR3(-1, 0, 1));
   TriangleMock* triangle2 = new TriangleMock(D3DXVECTOR3(-1, 1, -1), D3DXVECTOR3(1, 1, -1), D3DXVECTOR3(-1, 0, -1));
   geometry.push_back(triangle1);
   geometry.push_back(triangle2);

   BSPNodeTree<TriangleMock> tree(geometry);

   // query the tree for triangles from various positions
   Array<TriangleMock*> queryResult;

   tree.query(D3DXVECTOR3(0, 0, 2), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[1]);
   queryResult.clear();

   tree.query(D3DXVECTOR3(0, 0, 1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[1]);
   queryResult.clear();

   tree.query(D3DXVECTOR3(0, 0, 0), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[1]);
   queryResult.clear();

   tree.query(D3DXVECTOR3(0, 0, -1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[1]);
   queryResult.clear();

   tree.query(D3DXVECTOR3(0, 0, -2), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[1]);
   queryResult.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST(BSPNodeTree, coplanarPolygons)
{
    Array<TriangleMock*> geometry;
    TriangleMock* triangle1 = new TriangleMock(D3DXVECTOR3(-2, 1, 0), D3DXVECTOR3(-1, 1, 0), D3DXVECTOR3(-2, 0, 0));
    TriangleMock* triangle2 = new TriangleMock(D3DXVECTOR3( 1, 1, 0), D3DXVECTOR3( 2, 1, 0), D3DXVECTOR3( 1, 0, 0));
    geometry.push_back(triangle1);
    geometry.push_back(triangle2);

    BSPNodeTree<TriangleMock> tree(geometry);

    // query the tree for triangles from various positions
    Array<TriangleMock*> queryResult;

    tree.query(D3DXVECTOR3(0, 0, 1), queryResult);
    CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
    CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[0]);
    CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[1]);
    queryResult.clear();

    tree.query(D3DXVECTOR3(0, 0, -1), queryResult);
    CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
    CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[0]);
    CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[1]);
    queryResult.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST(BSPNodeTree, intersectingPolygons)
{
   Array<TriangleMock*> geometry;
   TriangleMock* triangle1 = new TriangleMock(D3DXVECTOR3(-1, 1, 0), D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(-1, 0, 0));
   TriangleMock* triangle2 = new TriangleMock(D3DXVECTOR3(0, 1, -1), D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, 0, -1));
   geometry.push_back(triangle1);
   geometry.push_back(triangle2);

   BSPNodeTree<TriangleMock> tree(geometry);

   // query the tree for triangles from various positions
   Array<TriangleMock*> queryResult;

   TriangleMock triangle2_bk_1(D3DXVECTOR3(0, 1, -1), D3DXVECTOR3(0, 0.5f, 0), D3DXVECTOR3(0, 0, 0));
   TriangleMock triangle2_bk_2(D3DXVECTOR3(0, 1, -1), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, -1));
   TriangleMock triangle2_fr_1(D3DXVECTOR3(0, 0.5f, 0), D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, 0, 0));

   tree.query(D3DXVECTOR3(1, 0, 1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle2_bk_1, *queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(triangle2_bk_2, *queryResult[1]);
   CPPUNIT_ASSERT_EQUAL(*triangle1,     *queryResult[2]);
   CPPUNIT_ASSERT_EQUAL(triangle2_fr_1, *queryResult[3]);
   queryResult.clear();

   tree.query(D3DXVECTOR3(1, 0, -1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle2_fr_1, *queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(*triangle1,     *queryResult[1]);
   CPPUNIT_ASSERT_EQUAL(triangle2_bk_1, *queryResult[2]);
   CPPUNIT_ASSERT_EQUAL(triangle2_bk_2, *queryResult[3]);
   queryResult.clear();

   tree.query(D3DXVECTOR3(-1, 0, -1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle2_fr_1, *queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(*triangle1,     *queryResult[1]);
   CPPUNIT_ASSERT_EQUAL(triangle2_bk_1, *queryResult[2]);
   CPPUNIT_ASSERT_EQUAL(triangle2_bk_2, *queryResult[3]);
   queryResult.clear();
}

///////////////////////////////////////////////////////////////////////////////
