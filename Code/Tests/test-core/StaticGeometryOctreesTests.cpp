#include "core-TestFramework\TestFramework.h"
#include "core\StaticGeometryOctree.h"
#include "core\BoundingSphere.h"
#include "core\Triangle.h"
#include "core\Vector.h"
#include <vector>
#include <map>
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class GeometricalObjectMock
   {
   private:
      BoundingSphere m_boundingSphere;
      std::vector<Triangle> m_geometry;

   public:
      GeometricalObjectMock()
         : m_boundingSphere(Vector(0, 0, 0), 0)
      {}

      const BoundingSphere& getBoundingVolume() const {return m_boundingSphere;}

      void addTriangle(const Triangle& tri)
      {
         m_geometry.push_back(tri);
         calculateBoundingVolume();
      }

      unsigned int getTrianglesCount() const 
      {
         return m_geometry.size();
      }

      const Triangle& getTriangle(unsigned int idx) const
      {
         return m_geometry.at(idx);
      }

      void split(const Plane& splitPlane,
                 GeometricalObjectMock** frontSplit,
                 GeometricalObjectMock** backSplit)
      {
         Array<Triangle*> front;
         Array<Triangle*> back;

         unsigned int trisCount = m_geometry.size();
         for (unsigned int i = 0; i < trisCount; ++i)
         {
            m_geometry[i].split(splitPlane, front, back);
         }

         trisCount = front.size();
         if (trisCount > 0)
         {
            *frontSplit = new GeometricalObjectMock();
            for (unsigned int i = 0; i < trisCount; ++i)
            {
               (*frontSplit)->addTriangle(*(front[i]));
               delete front[i];
            }
         }

         trisCount = back.size();
         if (trisCount > 0)
         {
            *backSplit = new GeometricalObjectMock();
            for (unsigned int i = 0; i < trisCount; ++i)
            {
               (*backSplit)->addTriangle(*(back[i]));
               delete back[i];
            }
         }
      }

   private:
      void calculateBoundingVolume()
      {
         Vector maxV(-FLT_MAX, -FLT_MAX, -FLT_MAX);
         Vector minV(FLT_MAX, FLT_MAX, FLT_MAX);

         unsigned int count = m_geometry.size();
         for (unsigned int i = 0; i < count; ++i)
         {
            for (unsigned int vtxIdx = 0; vtxIdx < 3; ++vtxIdx)
            {
               const Vector& vtx = m_geometry[i].vertex(vtxIdx);
               if (vtx.x > maxV.x) {maxV.x = vtx.x;}
               if (vtx.y > maxV.y) {maxV.y = vtx.y;}
               if (vtx.z > maxV.z) {maxV.z = vtx.z;}

               if (vtx.x < minV.x) {minV.x = vtx.x;}
               if (vtx.y < minV.y) {minV.y = vtx.y;}
               if (vtx.z < minV.z) {minV.z = vtx.z;}
            }
         }

         Vector dist;
         dist.setSub( maxV, minV );
         m_boundingSphere.origin.setMulAdd( dist, 0.5f, minV );
         m_boundingSphere.radius = dist.length() / 2.f;
      }
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST( StaticGeometryOctree, geometryCrossingSectorsBoundaryIsSplit )
{
   AABoundingBox treeBB(Vector(-10, -10, -10), Vector(10, 10, 10));
   int maxObjects = 1;
   int initDepth = 1;
   int maxDepth = 2;

   StaticGeometryOctree<GeometricalObjectMock> tree(treeBB, 
                                                    maxObjects, 
                                                    maxDepth,
                                                    initDepth);

   GeometricalObjectMock* object = new GeometricalObjectMock();
   object->addTriangle(Triangle(Vector(-1, 11, 10),
                                Vector( 1, 11, 10),
                                Vector(-1,  9, 10)));
   tree.insert(object);

   for ( uint i = 0; i < 1000; ++i )
   {
      CPPUNIT_ASSERT( true );
   }

   // let's query them
   Array<GeometricalObjectMock*> result;
   tree.query(BoundingSphere(Vector(2, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   COMPARE_VEC(Vector(0.5f, 10.5f, 10), result[0]->getBoundingVolume().origin);

   result.clear();
   tree.query(BoundingSphere(Vector(-2, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   COMPARE_VEC(Vector(-0.5f, 10, 10), result[0]->getBoundingVolume().origin);

   result.clear();
   tree.query(BoundingSphere(Vector(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, result.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryOctree, subdivisionOfAddedObjectsWhenNewViolatePartitioningLimits)
{
   AABoundingBox treeBB(Vector(-10, -10, -10), Vector(10, 10, 10));
   int maxObjects = 1;
   int maxDepth = 2;

   StaticGeometryOctree<GeometricalObjectMock> tree(treeBB, 
                                                    maxObjects, 
                                                    maxDepth);

   GeometricalObjectMock* object1 = new GeometricalObjectMock();
   object1->addTriangle(Triangle(Vector(-1, 11, 10),
                                 Vector( 1, 11, 10),
                                 Vector(-1,  9, 10)));
   GeometricalObjectMock* object2 = new GeometricalObjectMock();
   object2->addTriangle(Triangle(Vector( 8, 9, 10),
                                 Vector(10, 9, 10),
                                 Vector( 9, 9, 8)));


   // first object doesn't change anything - the limit is not violated,
   // so no subdivision takes place
   tree.insert(object1);
   // --> CPPUNIT_ASSERT_EQUAL((unsigned int)1, tree.getElementsCount());


   // but as soon as we add second object, a subdivision is performed,
   // and unfortunately it splits the first object in half
   tree.insert(object2);
   // --> CPPUNIT_ASSERT_EQUAL((unsigned int)3, tree.getElementsCount());

   // and finally let's do some querying just to be sure everything works
   Array<GeometricalObjectMock*> result;
   tree.query(BoundingSphere(Vector(2, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   COMPARE_VEC(Vector(0.5f, 10.5f, 10), result[0]->getBoundingVolume().origin);

   result.clear();
   tree.query(BoundingSphere(Vector(-2, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   COMPARE_VEC(Vector(-0.5f, 10, 10), result[0]->getBoundingVolume().origin);

   result.clear();
   tree.query(BoundingSphere(Vector(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, result.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryOctree, partitioningLimits)
{
   AABoundingBox treeBB(Vector(-10, -10, -10), Vector(10, 10, 10));
   int maxElementsPerSector = 1;
   int maxDepth = 0;

   StaticGeometryOctree<GeometricalObjectMock> tree(treeBB, 
                                                    maxElementsPerSector, 
                                                    maxDepth);

   GeometricalObjectMock* object1 = new GeometricalObjectMock();
   object1->addTriangle(Triangle(Vector(-1, 11, 10),
                                 Vector( 1, 11, 10),
                                 Vector(-1,  9, 10)));
   GeometricalObjectMock* object2 = new GeometricalObjectMock();
   object2->addTriangle(Triangle(Vector( 8, 9, 10),
                                 Vector(10, 9, 10),
                                 Vector( 9, 9, 8)));


   // first object doesn't change anything - the limit is not violated,
   // so no subdivision takes place
   tree.insert(object1);

   // --> CPPUNIT_ASSERT_EQUAL((unsigned int)1, tree.getElementsCount());

   // even though we're adding a second element - thus violating the maxElementsPerSector limit, 
   // maxDepth limit is stronger and prevents the tree from further subdivision (and thus the
   // subdivision of the objects the sector contains)
   tree.insert(object2);
   // --> CPPUNIT_ASSERT_EQUAL((unsigned int)2, tree.getElementsCount());

   // let's do some querying just to be sure everything works
   Array<GeometricalObjectMock*> result;
   tree.query(BoundingSphere(Vector(2, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   COMPARE_VEC(Vector(0, 10, 10), result[0]->getBoundingVolume().origin);

   result.clear();
   tree.query(BoundingSphere(Vector(-2, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   COMPARE_VEC(Vector(0, 10, 10), result[0]->getBoundingVolume().origin);

   result.clear();
   tree.query(BoundingSphere(Vector(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryOctree, removingObjects)
{
   AABoundingBox treeBB(Vector(-10, -10, -10), Vector(10, 10, 10));
   int maxElementsPerSector = 1;
   int maxDepth = 0;

   StaticGeometryOctree<GeometricalObjectMock> tree(treeBB, 
                                                    maxElementsPerSector, 
                                                    maxDepth);

   GeometricalObjectMock* object = new GeometricalObjectMock();
   object->addTriangle(Triangle(Vector(-1, 11, 10),
                                Vector( 1, 11, 10),
                                Vector(-1,  9, 10)));

   SGHandle objectHandle = tree.insert(object);

   Array<GeometricalObjectMock*> result;
   tree.query(BoundingSphere(Vector(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());

   tree.remove(objectHandle);
   result.clear();
   tree.query(BoundingSphere(Vector(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, result.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryOctree, addingObjectWithBoundingBoxOverTwoSectorsAndGeometryOnlyInOne)
{
   AABoundingBox treeBB(Vector(-10, -10, -10), Vector(10, 10, 10));
   int maxElementsPerSector = 1;
   int maxDepth = 1;
   int initialDepth = 1;

   StaticGeometryOctree<GeometricalObjectMock> tree(treeBB, 
                                                    maxElementsPerSector, 
                                                    maxDepth,
                                                    initialDepth);

   GeometricalObjectMock* object = new GeometricalObjectMock();
   object->addTriangle(Triangle(Vector(-1, 11,  9),
                                Vector(-1, 11, 11),
                                Vector(-1,  3,  9)));

   tree.insert(object);

   Array<GeometricalObjectMock*> result;
   tree.query(BoundingSphere(Vector(-2, 10, 10), 1), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());

   result.clear();
   tree.query(BoundingSphere(Vector(2, 10, 10), 1), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, result.size());
}

///////////////////////////////////////////////////////////////////////////////
