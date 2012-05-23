#include "core-TestFramework\TestFramework.h"
#include "NodeA.h"
#include "core\TNodesSpatialStorage.h"
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
typedef TNodesSpatialStorage<NodeA, LinearStorage> TestStorage;

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(TNodesSpatialStorage, broadPhaseQueries)
{
   TestStorage storage(new SolidTestStorage());

   NodeA* staticNodeInsideQuery = new NodeA();
   NodeA* staticNodeOutsideNode = new NodeA();
   NodeA* dynamicNodeInsideNode = new NodeA();
   NodeA* dynamicNodeOutsideNode = new NodeA();

   staticNodeInsideQuery->setBoundingVolume(new BoundingSphere(Vector(0, 0, 0), 1));
   staticNodeOutsideNode->setBoundingVolume(new BoundingSphere(Vector(0, 0, 0), 1));
   dynamicNodeInsideNode->setBoundingVolume(new BoundingSphere(Vector(0, 0, 0), 1));
   dynamicNodeOutsideNode->setBoundingVolume(new BoundingSphere(Vector(0, 0, 0), 1));
   staticNodeOutsideNode->accessLocalMtx().setTranslation( Vector( 0, 0, -10 ) );
   dynamicNodeOutsideNode->accessLocalMtx().setTranslation( Vector( 0, 0, -10 ) );

   storage.addNode(staticNodeInsideQuery);
   storage.addNode(staticNodeOutsideNode);
   storage.addNode(dynamicNodeInsideNode);
   storage.addNode(dynamicNodeOutsideNode);

   Array<NodeA*> nodes;
   storage.query(Ray(Vector(0, 0, 0.5f), Vector(0, 0, 1)), nodes);

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, nodes.size());
   CPPUNIT_ASSERT_EQUAL(staticNodeInsideQuery, nodes[0]);
   CPPUNIT_ASSERT_EQUAL(dynamicNodeInsideNode, nodes[1]);
}

///////////////////////////////////////////////////////////////////////////////
