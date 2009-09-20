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

   // -------------------------------------------------------------------------

   class ResultingGeometryStorageMock
   {
   private:
      std::vector<GeometricalObjectMock*> m_objects;

   public:
      ~ResultingGeometryStorageMock()
      {
         unsigned int count = m_objects.size();
         for (unsigned int i = 0; i < count; ++i)
         {
            delete m_objects[i];
         }
      }

      void getObjects(Array<GeometricalObjectMock*>& output)
      {
         unsigned int count = m_objects.size();
         for (unsigned int i = 0; i < count; ++i)
         {
            output.push_back(m_objects[i]);
         }
      }

      void manage(GeometricalObjectMock* obj)
      {
         m_objects.push_back(obj);
      }

      void unmanage(GeometricalObjectMock& obj)
      {
         unsigned int count = m_objects.size();
         for (std::vector<GeometricalObjectMock*>::iterator it = m_objects.begin();
              it != m_objects.end(); ++it)
         {
            if (&obj == *it)
            {
               delete *it;
               m_objects.erase(it);
               break;
            }
         }
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

   ResultingGeometryStorageMock resultingGeometryStorage;
   StaticGeometryOctree<GeometricalObjectMock, ResultingGeometryStorageMock> tree(treeBB, 
                                                    maxObjects, 
                                                    maxDepth,
                                                    resultingGeometryStorage,
                                                    initDepth);

   GeometricalObjectMock object;
   object.addTriangle(Triangle(D3DXVECTOR3(-1, 11, 10),
                               D3DXVECTOR3( 1, 11, 10),
                               D3DXVECTOR3(-1,  9, 10)));
   tree.insert(object);

   // after the object's been added to the tree, we should receive
   // two resulting objects
   Array<GeometricalObjectMock*> result;
   resultingGeometryStorage.getObjects(result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, result.size());

   GeometricalObjectMock& clippedObj1 = *(result[0]);
   GeometricalObjectMock& clippedObj2 = *(result[1]);

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
   result.clear();
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

   ResultingGeometryStorageMock resultingGeometryStorage;
   StaticGeometryOctree<GeometricalObjectMock, ResultingGeometryStorageMock> tree(treeBB, 
                                                    maxObjects, 
                                                    maxDepth,
                                                    resultingGeometryStorage);

   GeometricalObjectMock object1;
   object1.addTriangle(Triangle(D3DXVECTOR3(-1, 11, 10),
                                D3DXVECTOR3( 1, 11, 10),
                                D3DXVECTOR3(-1,  9, 10)));
   GeometricalObjectMock object2;
   object2.addTriangle(Triangle(D3DXVECTOR3( 8, 9, 10),
                                D3DXVECTOR3(10, 9, 10),
                                D3DXVECTOR3( 9, 9, 8)));


   // first object doesn't change anything - the limit is not violated,
   // so no subdivision takes place
   tree.insert(object1);

   Array<GeometricalObjectMock*> result;
   resultingGeometryStorage.getObjects(result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, result.size());


   // but as soon as we add second object, a subdivision is performed,
   // and unfortunately it splits the first object in half
   tree.insert(object2);
   result.clear();
   resultingGeometryStorage.getObjects(result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, result.size());

   GeometricalObjectMock& clippedObj1 = *(result[0]);
   GeometricalObjectMock& clippedObj2 = *(result[1]);

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
   result.clear();
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

   ResultingGeometryStorageMock resultingGeometryStorage;
   StaticGeometryOctree<GeometricalObjectMock, ResultingGeometryStorageMock> tree(treeBB, 
                                                    maxElementsPerSector, 
                                                    maxDepth,
                                                    resultingGeometryStorage);

   GeometricalObjectMock object1;
   object1.addTriangle(Triangle(D3DXVECTOR3(-1, 11, 10),
                                D3DXVECTOR3( 1, 11, 10),
                                D3DXVECTOR3(-1,  9, 10)));
   GeometricalObjectMock object2;
   object2.addTriangle(Triangle(D3DXVECTOR3( 8, 9, 10),
                                D3DXVECTOR3(10, 9, 10),
                                D3DXVECTOR3( 9, 9, 8)));


   // first object doesn't change anything - the limit is not violated,
   // so no subdivision takes place
   tree.insert(object1);

   Array<GeometricalObjectMock*> result;
   resultingGeometryStorage.getObjects(result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, result.size());

   // even though we're adding a second element - thus violationg the maxElementsPerSector limit, 
   // maxDepth limit is stronger and prevents the tree from further subdivision (and thus the
   // subdivision of the objects the sector contains)
   tree.insert(object2);
   result.clear();
   resultingGeometryStorage.getObjects(result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, result.size());

   // let's do some querying just to be sure everything works
   result.clear();
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

   ResultingGeometryStorageMock resultingGeometryStorage;
   StaticGeometryOctree<GeometricalObjectMock, ResultingGeometryStorageMock> tree(treeBB, 
                                                    maxElementsPerSector, 
                                                    maxDepth,
                                                    resultingGeometryStorage);

   GeometricalObjectMock object;
   object.addTriangle(Triangle(D3DXVECTOR3(-1, 11, 10),
                               D3DXVECTOR3( 1, 11, 10),
                               D3DXVECTOR3(-1,  9, 10)));

   tree.insert(object);

   Array<GeometricalObjectMock*> result;
   tree.query(BoundingSphere(D3DXVECTOR3(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());

   tree.remove(object);
   result.clear();
   tree.query(BoundingSphere(D3DXVECTOR3(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, result.size());

   tree.insert(object);
   result.clear();
   tree.query(BoundingSphere(D3DXVECTOR3(0, 10, 10), 1.5f), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryOctree, addingObjectWithBoundingBoxOverTwoSectorsAndGeometryOnlyInOne)
{
   AABoundingBox treeBB(D3DXVECTOR3(-10, -10, -10), D3DXVECTOR3(10, 10, 10));
   int maxElementsPerSector = 1;
   int maxDepth = 1;
   int initialDepth = 1;

   ResultingGeometryStorageMock resultingGeometryStorage;
   StaticGeometryOctree<GeometricalObjectMock, ResultingGeometryStorageMock> tree(treeBB, 
                                                    maxElementsPerSector, 
                                                    maxDepth,
                                                    resultingGeometryStorage,
                                                    initialDepth);

   GeometricalObjectMock object;
   object.addTriangle(Triangle(D3DXVECTOR3(-1, 11,  9),
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
