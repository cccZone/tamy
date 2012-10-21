#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\DynamicOctree.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class BoundedObjectMock
   {
   private:
      BoundingSphere m_boundingSphere;

   public:
      BoundedObjectMock(float ox, float oy, float oz, float rad)
         : m_boundingSphere(Vector(ox, oy, oz), rad)
      {}

      const BoundingSphere& getBoundingVolume() const {return m_boundingSphere;}

      void move(float ox, float oy, float oz)
      {
         m_boundingSphere.origin[0] = ox;
         m_boundingSphere.origin[1] = oy;
         m_boundingSphere.origin[2] = oz;
      }
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(DynamicOctree, addingObjects)
{
   AABoundingBox treeBB(Vector(-10, -10, -10), Vector(10, 10, 10));
   int depth = 1;
   DynamicOctree<BoundedObjectMock> tree(treeBB, depth);
   Array<BoundedObjectMock*> result;

   BoundedObjectMock ob1(5, 5, 5, 1);
   tree.insert(ob1);

   Vector searchLocation(5, 5, 5);

   // query finally finds the object
   tree.query(BoundingSphere(searchLocation, 3), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   CPPUNIT_ASSERT_EQUAL(&ob1, result[0]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(DynamicOctree, queryingElements)
{
   AABoundingBox treeBB(Vector(-10, -10, -10), Vector(10, 10, 10));
   int depth = 1;
   DynamicOctree<BoundedObjectMock> tree(treeBB, depth);
   Array<BoundedObjectMock*> result;

   BoundedObjectMock ob1(5, 5, 5, 1);
   tree.insert(ob1);

   Vector location1(4, 4, 4);
   Vector location2(-4, -4, -4);

   // let's check where the object is before we move it
   // to verify the queries work correctly
   tree.query(BoundingSphere(location1, 1), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   CPPUNIT_ASSERT_EQUAL(&ob1, result[0]);

   result.clear();
   tree.query(BoundingSphere(location2, 1), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, result.size());

   // now let's move the object
   ob1.move(-5, -5, -5);
   tree.update(ob1);

   result.clear();
   tree.query(BoundingSphere(location1, 1), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, result.size());

   result.clear();
   tree.query(BoundingSphere(location2, 1), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   CPPUNIT_ASSERT_EQUAL(&ob1, result[0]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(DynamicOctree, removingElementsBetweenUpdates)
{
   AABoundingBox treeBB(Vector(-10, -10, -10), Vector(10, 10, 10));
   int depth = 1;
   DynamicOctree<BoundedObjectMock> tree(treeBB, depth);
   Array<BoundedObjectMock*> result;

   BoundedObjectMock ob1(5, 5, 5, 1);
   tree.insert(ob1);

   Vector searchLocation(5, 5, 5);

   // let's check where the object is before we move it
   // to verify the queries work correctly
   tree.query(BoundingSphere(searchLocation, 1), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, result.size());
   CPPUNIT_ASSERT_EQUAL(&ob1, result[0]);

   // now let's remove the object, but without updating the tree...
   tree.remove(ob1);

   // ...and the query fails to find it
   result.clear();
   tree.query(BoundingSphere(searchLocation, 0), result);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, result.size());
}

///////////////////////////////////////////////////////////////////////////////
