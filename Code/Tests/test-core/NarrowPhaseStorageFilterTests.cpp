#include "core-TestFramework\TestFramework.h"
#include "NodeA.h"
#include "core\NarrowPhaseStorageFilter.h"
#include "core\LinearStorage.h"
#include "core-Renderer\Camera.h"
#include "core\Triangle.h"
#include "core-TestFramework\MatrixWriter.h"
#include "core\Ray.h"
#include "core\Array.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   typedef LinearStorage<NodeA> SolidTestStorage;
} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(NarrowPhaseStorageFilter, narrowPhaseQueries)
{
   SolidTestStorage* underlyingSolidStorage = new SolidTestStorage();
   NarrowPhaseStorageFilter<NodeA> storage(underlyingSolidStorage);

   NodeA node;
   node.addTriangle(new Triangle(Vector(-3, 1, 0), Vector(3, 1, 0), Vector(-3, -1, 0)));
   node.setBoundingVolume(new BoundingSphere(Vector(0, 0, 0), 3));
   node.accessLocalMtx().setTranslation( Vector( 0, 0, 10 ) );
   underlyingSolidStorage->insert(node);

   // run the tests
   Array<NodeA*> nodes;

   nodes.clear();
   storage.query(Ray(Vector(-1, 0, 0), Vector(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, nodes.size());
   CPPUNIT_ASSERT_EQUAL(&node, nodes[0]);

   nodes.clear();
   storage.query(Ray(Vector(1, 0, 0), Vector(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());

   // the object has been  lifted a bit so that its bounding volume will still
   // intersect the ray, but its silhouette won't - now check to see if this
   // transformation change gets included in the query results
   node.accessLocalMtx().setTranslation( Vector( 0, 1, 10 ) );
   nodes.clear();
   storage.query(Ray(Vector(-1, 0, 0), Vector(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());
}

///////////////////////////////////////////////////////////////////////////////
