#include "core-TestFramework\TestFramework.h"
#include "core\Array.h"
#include "core\PointVolume.h"
#include "core\TriangleSplitter.h"
#include "core-TestFramework\MatrixWriter.h"
#include "core\BoundingVolume.h"
#include "core\BSPNodeTree.h"
#include "core\FastFloat.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

   struct TriangleMock : public SplittableTriangle<Vector>
   {
      DECLARE_ALLOCATOR( TriangleMock, AM_ALIGNED_16 );

      Vector v[3];

      TriangleMock(const Vector& v1, const Vector& v2, const Vector& v3)
      {
         v[0] = v1;
         v[1] = v2;
         v[2] = v3;
      }

      void getPlane( Plane& outPlane ) const
      {
         outPlane.setFromPoints( v[0], v[2], v[1] );
      }

      PlaneClassification classifyAgainst(const Plane& plane)
      {
         const FastFloat dist1 = plane.dotCoord( v[0] );
         const FastFloat dist2 = plane.dotCoord( v[1] );
         const FastFloat dist3 = plane.dotCoord( v[2] );

         if ( ( dist1 < Float_0 ) && ( dist2 < Float_0 ) && ( dist3 < Float_0 ) )
         {
            return PPC_BACK;
         }
         else if ( ( dist1 > Float_0 ) && ( dist2 > Float_0 ) && ( dist3 > Float_0) )
         {
            return PPC_FRONT;
         }
         else if ( ( dist1 == Float_0 ) && ( dist2 == Float_0 ) && ( dist3 == Float_0 ) )
         {
            return PPC_COPLANAR;
         }
         else
         {
            return PPC_SPANNING;
         }
      }

      const Vector& vertex(unsigned int vtxIdx) const {return v[vtxIdx];}

      const Vector& vertexPos(unsigned int vtxIdx) const {return v[vtxIdx];}

      void splitEdge( const FastFloat& percentage, unsigned int startVtxIdx, unsigned int endVtxIdx, Vector& outVertex ) const
      {
         const Vector& v1 = v[startVtxIdx];

         Vector edge;
         edge.setSub( v[endVtxIdx], v1 );

         outVertex.setMulAdd( edge, percentage, v1 );
      }

      void split( const Plane& plane, Array<TriangleMock*>& frontSplit, Array<TriangleMock*>& backSplit)
      {
         TriangleSplitter<Vector, TriangleMock> splitter;
         splitter.split(*this, plane, frontSplit, backSplit);
      }

      bool operator==(const TriangleMock& rhs) const
      {
         bool match = true;
         Vector tmpDir;
         for (int i = 0; i < 3; ++i)
         {
            tmpDir.setSub( v[i], rhs.v[i] );
            const FastFloat dist = tmpDir.length();
            if (dist > Float_1e_3) 
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
   TriangleMock* triangle = new TriangleMock(Vector(-1, 1, 0), Vector(1, 1, 0), Vector(-1, 0, 0));
   geometry.push_back(triangle);

   BSPNodeTree<TriangleMock> tree(geometry);

   // query the tree for triangles from various positions
   Array<TriangleMock*> queryResult;

   tree.query(Vector(0, 0, -1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, queryResult.size());
   queryResult.clear();

   tree.query(Vector(0, 0, 1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, queryResult.size());
   queryResult.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST(BSPNodeTree, multipleTrianglesNonSplitting)
{
   Array<TriangleMock*> geometry;
   TriangleMock* triangle1 = new TriangleMock(Vector(-1, 1, 1), Vector(1, 1, 1), Vector(-1, 0, 1));
   TriangleMock* triangle2 = new TriangleMock(Vector(-1, 1, -1), Vector(1, 1, -1), Vector(-1, 0, -1));
   geometry.push_back(triangle1);
   geometry.push_back(triangle2);

   BSPNodeTree<TriangleMock> tree(geometry);

   // query the tree for triangles from various positions
   Array<TriangleMock*> queryResult;

   tree.query(Vector(0, 0, 2), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[1]);
   queryResult.clear();

   tree.query(Vector(0, 0, 1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[1]);
   queryResult.clear();

   tree.query(Vector(0, 0, 0), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[1]);
   queryResult.clear();

   tree.query(Vector(0, 0, -1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[1]);
   queryResult.clear();

   tree.query(Vector(0, 0, -2), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[1]);
   queryResult.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST(BSPNodeTree, coplanarPolygons)
{
    Array<TriangleMock*> geometry;
    TriangleMock* triangle1 = new TriangleMock(Vector(-2, 1, 0), Vector(-1, 1, 0), Vector(-2, 0, 0));
    TriangleMock* triangle2 = new TriangleMock(Vector( 1, 1, 0), Vector( 2, 1, 0), Vector( 1, 0, 0));
    geometry.push_back(triangle1);
    geometry.push_back(triangle2);

    BSPNodeTree<TriangleMock> tree(geometry);

    // query the tree for triangles from various positions
    Array<TriangleMock*> queryResult;

    tree.query(Vector(0, 0, 1), queryResult);
    CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
    CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[0]);
    CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[1]);
    queryResult.clear();

    tree.query(Vector(0, 0, -1), queryResult);
    CPPUNIT_ASSERT_EQUAL((unsigned int)2, queryResult.size());
    CPPUNIT_ASSERT_EQUAL(triangle1, queryResult[0]);
    CPPUNIT_ASSERT_EQUAL(triangle2, queryResult[1]);
    queryResult.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST(BSPNodeTree, intersectingPolygons)
{
   Array<TriangleMock*> geometry;
   TriangleMock* triangle1 = new TriangleMock(Vector(-1, 1, 0), Vector(1, 0, 0), Vector(-1, 0, 0));
   TriangleMock* triangle2 = new TriangleMock(Vector(0, 1, -1), Vector(0, 0, 1), Vector(0, 0, -1));
   geometry.push_back(triangle1);
   geometry.push_back(triangle2);

   BSPNodeTree<TriangleMock> tree(geometry);

   // query the tree for triangles from various positions
   Array<TriangleMock*> queryResult;

   TriangleMock triangle2_bk_1(Vector(0, 1, -1), Vector(0, 0.5f, 0), Vector(0, 0, 0));
   TriangleMock triangle2_bk_2(Vector(0, 1, -1), Vector(0, 0, 0), Vector(0, 0, -1));
   TriangleMock triangle2_fr_1(Vector(0, 0.5f, 0), Vector(0, 0, 1), Vector(0, 0, 0));

   tree.query(Vector(1, 0, 1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle2_bk_1, *queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(triangle2_bk_2, *queryResult[1]);
   CPPUNIT_ASSERT_EQUAL(*triangle1,     *queryResult[2]);
   CPPUNIT_ASSERT_EQUAL(triangle2_fr_1, *queryResult[3]);
   queryResult.clear();

   tree.query(Vector(1, 0, -1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle2_fr_1, *queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(*triangle1,     *queryResult[1]);
   CPPUNIT_ASSERT_EQUAL(triangle2_bk_1, *queryResult[2]);
   CPPUNIT_ASSERT_EQUAL(triangle2_bk_2, *queryResult[3]);
   queryResult.clear();

   tree.query(Vector(-1, 0, -1), queryResult);
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, queryResult.size());
   CPPUNIT_ASSERT_EQUAL(triangle2_fr_1, *queryResult[0]);
   CPPUNIT_ASSERT_EQUAL(*triangle1,     *queryResult[1]);
   CPPUNIT_ASSERT_EQUAL(triangle2_bk_1, *queryResult[2]);
   CPPUNIT_ASSERT_EQUAL(triangle2_bk_2, *queryResult[3]);
   queryResult.clear();
}

///////////////////////////////////////////////////////////////////////////////
