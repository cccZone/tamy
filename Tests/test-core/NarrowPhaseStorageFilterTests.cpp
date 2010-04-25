#include "core-TestFramework\TestFramework.h"
#include "NodeA.h"
#include "core\NarrowPhaseStorageFilter.h"
#include "core\LinearStorage.h"
#include "core-Renderer\Camera.h"
#include "core\Triangle.h"
#include "core\MatrixWriter.h"
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
   node.addTriangle(new Triangle(D3DXVECTOR3(-3, 1, 0), D3DXVECTOR3(3, 1, 0), D3DXVECTOR3(-3, -1, 0)));
   node.setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 3));
   D3DXMatrixTranslation(&(node.accessLocalMtx()), 0, 0, 10);
   underlyingSolidStorage->insert(node);

   // run the tests
   Array<NodeA*> nodes;

   nodes.clear();
   storage.query(Ray(D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, nodes.size());
   CPPUNIT_ASSERT_EQUAL(&node, nodes[0]);

   nodes.clear();
   storage.query(Ray(D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());

   // the object has been  lifted a bit so that its bounding volume will still
   // intersect the ray, but its silhouette won't - now check to see if this
   // transformation change gets included in the query results
   D3DXMatrixTranslation(&(node.accessLocalMtx()), 0, 1, 10);
   nodes.clear();
   storage.query(Ray(D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());
}

///////////////////////////////////////////////////////////////////////////////
