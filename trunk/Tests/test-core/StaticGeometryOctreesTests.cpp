#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\StaticGeometryOctree.h"
#include "core\BoundingSphere.h"
#include "core\Triangle.h"
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
         : m_boundingSphere(D3DXVECTOR3(0, 0, 0), 0)
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

      void split(const D3DXPLANE& splitPlane,
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
         D3DXVECTOR3 max = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
         D3DXVECTOR3 min = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

         unsigned int count = m_geometry.size();
         for (unsigned int i = 0; i < count; ++i)
         {
            for (unsigned int vtxIdx = 0; vtxIdx < 3; ++vtxIdx)
            {
               const D3DXVECTOR3& vtx = m_geometry[i].vertex(vtxIdx);
               if (vtx.x > max.x) {max.x = vtx.x;}
               if (vtx.y > max.y) {max.y = vtx.y;}
               if (vtx.z > max.z) {max.z = vtx.z;}

               if (vtx.x < min.x) {min.x = vtx.x;}
               if (vtx.y < min.y) {min.y = vtx.y;}
               if (vtx.z < min.z) {min.z = vtx.z;}
            }
         }

         D3DXVECTOR3 dist = max - min;
         m_boundingSphere.origin = min + (dist / 2.f);
         m_boundingSphere.radius = D3DXVec3Length(&dist) / 2.f;
      }
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryOctree, geometryCrossingSectorsBoundaryIsSplit)
{
   AABoundingBox treeBB(D3DXVECTOR3(-10, -10, -10), D3DXVECTOR3(10, 10, 10));
   int maxObjects = 1;
   int initDepth = 1;
   int maxDepth = 2;

   StaticGeometryOctree<GeometricalObjectMock> tree(treeBB, 
                                                    maxObjects, 
                                                    maxDepth,
                                                    initDepth);

   GeometricalObjectMock* object = new GeometricalObjectMock();
   object->addTriangle(Triangle(D3DXVECTOR3(-1, 11, 10),
                                D3DXVECTOR3( 1, 11, 10),
                                D3DXVECTOR3(-1,  9, 10)));
   tree.insert(object);

   // after the object's been added to the tree, we should receive
   // two resulting objects
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, tree.getElementsCount());

   GeometricalObjectMock& clippedObj1 = tree.getElement(0);
   GeometricalObjectMock& clippedObj2 = tree.getElement(1);

   // their bounding volumes should be clipped accordingly
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.707f, clippedObj1.getBoundingVolume().radius, 0.001f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.118f, clippedObj2.getBoundingVolume().radius, 0.001f);
   COMPARE_VEC(D3DXVECTOR3( 0.5f, 10.5f, 10), clippedObj1.getBoundingVolume().origin);
   COMPARE_VEC(D3DXVECTOR3(-0.5f, 10, 10), clippedObj2.getBoundingVolume().origin);

   // as well as the underlying properties
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, clippedObj1.getTrianglesCount());
   const Triangle& obj1Tri = clippedObj1.getTriangle(0);
   COMPARE_VEC(D3DXVECTOR3( 0, 11, 10), obj1Tri.vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 1, 11, 10), obj1Tri.vertex(1));
   COMPARE_VEC(D3DXVECTOR3( 0, 10, 10), obj1Tri.vertex(2));

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, clippedObj2.getTrianglesCount());
   const Triangle& obj2Tri1 = clippedObj2.getTriangle(0);
   COMPARE_VEC(D3DXVECTOR3(-1, 11, 10), obj2Tri1.vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 0, 11, 10), obj2Tri1.vertex(1));
   COMPARE_VEC(D3DXVECTOR3( 0, 10, 10), obj2Tri1.vertex(2));

   const Triangle& obj2Tri2 = clippedObj2.getTriangle(1);
   COMPARE_VEC(D3DXVECTOR3(-1, 11, 10), obj2Tri2.vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 0, 10, 10), obj2Tri2.vertex(1));
   COMPARE_VEC(D3DXVECTOR3(-1,  9, 10), obj2Tri2.vertex(2));

   // let's query them
   Array<GeometricalObjectMock*> result;
   tree.query(BoundingSphere(D3DXVECTOR3(2, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   COMPARE_VEC(D3DXVECTOR3(0.5f, 10.5f, 10), result[0]->getBoundingVolume().origin);

   result.clear();
   tree.query(BoundingSphere(D3DXVECTOR3(-2, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   COMPARE_VEC(D3DXVECTOR3(-0.5f, 10, 10), result[0]->getBoundingVolume().origin);

   result.clear();
   tree.query(BoundingSphere(D3DXVECTOR3(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, result.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryOctree, subdivisionOfAddedObjectsWhenNewViolatePartitioningLimits)
{
   AABoundingBox treeBB(D3DXVECTOR3(-10, -10, -10), D3DXVECTOR3(10, 10, 10));
   int maxObjects = 1;
   int maxDepth = 2;

   StaticGeometryOctree<GeometricalObjectMock> tree(treeBB, 
                                                    maxObjects, 
                                                    maxDepth);

   GeometricalObjectMock* object1 = new GeometricalObjectMock();
   object1->addTriangle(Triangle(D3DXVECTOR3(-1, 11, 10),
                                 D3DXVECTOR3( 1, 11, 10),
                                 D3DXVECTOR3(-1,  9, 10)));
   GeometricalObjectMock* object2 = new GeometricalObjectMock();
   object2->addTriangle(Triangle(D3DXVECTOR3( 8, 9, 10),
                                 D3DXVECTOR3(10, 9, 10),
                                 D3DXVECTOR3( 9, 9, 8)));


   // first object doesn't change anything - the limit is not violated,
   // so no subdivision takes place
   tree.insert(object1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, tree.getElementsCount());


   // but as soon as we add second object, a subdivision is performed,
   // and unfortunately it splits the first object in half
   tree.insert(object2);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, tree.getElementsCount());

   GeometricalObjectMock& clippedObj1 = tree.getElement(0);
   GeometricalObjectMock& clippedObj2 = tree.getElement(2);

   // their bounding volumes should be clipped accordingly
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.707f, clippedObj1.getBoundingVolume().radius, 0.001f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.118f, clippedObj2.getBoundingVolume().radius, 0.001f);
   COMPARE_VEC(D3DXVECTOR3( 0.5f, 10.5f, 10), clippedObj1.getBoundingVolume().origin);
   COMPARE_VEC(D3DXVECTOR3(-0.5f, 10, 10), clippedObj2.getBoundingVolume().origin);

   // as well as the underlying properties
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, clippedObj1.getTrianglesCount());
   const Triangle& obj1Tri = clippedObj1.getTriangle(0);
   COMPARE_VEC(D3DXVECTOR3( 0, 11, 10), obj1Tri.vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 1, 11, 10), obj1Tri.vertex(1));
   COMPARE_VEC(D3DXVECTOR3( 0, 10, 10), obj1Tri.vertex(2));

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, clippedObj2.getTrianglesCount());
   const Triangle& obj2Tri1 = clippedObj2.getTriangle(0);
   COMPARE_VEC(D3DXVECTOR3(-1, 11, 10), obj2Tri1.vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 0, 11, 10), obj2Tri1.vertex(1));
   COMPARE_VEC(D3DXVECTOR3( 0, 10, 10), obj2Tri1.vertex(2));

   const Triangle& obj2Tri2 = clippedObj2.getTriangle(1);
   COMPARE_VEC(D3DXVECTOR3(-1, 11, 10), obj2Tri2.vertex(0));
   COMPARE_VEC(D3DXVECTOR3( 0, 10, 10), obj2Tri2.vertex(1));
   COMPARE_VEC(D3DXVECTOR3(-1,  9, 10), obj2Tri2.vertex(2));

   // and finally let's do some querying just to be sure everything works
   Array<GeometricalObjectMock*> result;
   tree.query(BoundingSphere(D3DXVECTOR3(2, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   COMPARE_VEC(D3DXVECTOR3(0.5f, 10.5f, 10), result[0]->getBoundingVolume().origin);

   result.clear();
   tree.query(BoundingSphere(D3DXVECTOR3(-2, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   COMPARE_VEC(D3DXVECTOR3(-0.5f, 10, 10), result[0]->getBoundingVolume().origin);

   result.clear();
   tree.query(BoundingSphere(D3DXVECTOR3(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, result.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryOctree, partitioningLimits)
{
   AABoundingBox treeBB(D3DXVECTOR3(-10, -10, -10), D3DXVECTOR3(10, 10, 10));
   int maxElementsPerSector = 1;
   int maxDepth = 0;

   StaticGeometryOctree<GeometricalObjectMock> tree(treeBB, 
                                                    maxElementsPerSector, 
                                                    maxDepth);

   GeometricalObjectMock* object1 = new GeometricalObjectMock();
   object1->addTriangle(Triangle(D3DXVECTOR3(-1, 11, 10),
                                 D3DXVECTOR3( 1, 11, 10),
                                 D3DXVECTOR3(-1,  9, 10)));
   GeometricalObjectMock* object2 = new GeometricalObjectMock();
   object2->addTriangle(Triangle(D3DXVECTOR3( 8, 9, 10),
                                 D3DXVECTOR3(10, 9, 10),
                                 D3DXVECTOR3( 9, 9, 8)));


   // first object doesn't change anything - the limit is not violated,
   // so no subdivision takes place
   tree.insert(object1);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, tree.getElementsCount());

   // even though we're adding a second element - thus violating the maxElementsPerSector limit, 
   // maxDepth limit is stronger and prevents the tree from further subdivision (and thus the
   // subdivision of the objects the sector contains)
   tree.insert(object2);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, tree.getElementsCount());

   // let's do some querying just to be sure everything works
   Array<GeometricalObjectMock*> result;
   tree.query(BoundingSphere(D3DXVECTOR3(2, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   COMPARE_VEC(D3DXVECTOR3(0, 10, 10), result[0]->getBoundingVolume().origin);

   result.clear();
   tree.query(BoundingSphere(D3DXVECTOR3(-2, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   COMPARE_VEC(D3DXVECTOR3(0, 10, 10), result[0]->getBoundingVolume().origin);

   result.clear();
   tree.query(BoundingSphere(D3DXVECTOR3(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryOctree, removingObjects)
{
   AABoundingBox treeBB(D3DXVECTOR3(-10, -10, -10), D3DXVECTOR3(10, 10, 10));
   int maxElementsPerSector = 1;
   int maxDepth = 0;

   StaticGeometryOctree<GeometricalObjectMock> tree(treeBB, 
                                                    maxElementsPerSector, 
                                                    maxDepth);

   GeometricalObjectMock* object = new GeometricalObjectMock();
   object->addTriangle(Triangle(D3DXVECTOR3(-1, 11, 10),
                                D3DXVECTOR3( 1, 11, 10),
                                D3DXVECTOR3(-1,  9, 10)));

   SGHandle objectHandle = tree.insert(object);

   Array<GeometricalObjectMock*> result;
   tree.query(BoundingSphere(D3DXVECTOR3(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());

   tree.remove(objectHandle);
   result.clear();
   tree.query(BoundingSphere(D3DXVECTOR3(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, result.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryOctree, addingObjectWithBoundingBoxOverTwoSectorsAndGeometryOnlyInOne)
{
   AABoundingBox treeBB(D3DXVECTOR3(-10, -10, -10), D3DXVECTOR3(10, 10, 10));
   int maxElementsPerSector = 1;
   int maxDepth = 1;
   int initialDepth = 1;

   StaticGeometryOctree<GeometricalObjectMock> tree(treeBB, 
                                                    maxElementsPerSector, 
                                                    maxDepth,
                                                    initialDepth);

   GeometricalObjectMock* object = new GeometricalObjectMock();
   object->addTriangle(Triangle(D3DXVECTOR3(-1, 11,  9),
                                D3DXVECTOR3(-1, 11, 11),
                                D3DXVECTOR3(-1,  3,  9)));

   tree.insert(object);

   Array<GeometricalObjectMock*> result;
   tree.query(BoundingSphere(D3DXVECTOR3(-2, 10, 10), 1), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());

   result.clear();
   tree.query(BoundingSphere(D3DXVECTOR3(2, 10, 10), 1), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, result.size());
}

///////////////////////////////////////////////////////////////////////////////
