#include "core-TestFramework\TestFramework.h"
#include "NodeA.h"
#include "core\TNodesSpatialStorage.h"
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

   staticNodeInsideQuery->setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 1));
   staticNodeOutsideNode->setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 1));
   dynamicNodeInsideNode->setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 1));
   dynamicNodeOutsideNode->setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 1));
   D3DXMatrixTranslation(&(staticNodeOutsideNode->accessLocalMtx()), 0, 0, -10);
   D3DXMatrixTranslation(&(dynamicNodeOutsideNode->accessLocalMtx()), 0, 0, -10);

   storage.addNode(staticNodeInsideQuery);
   storage.addNode(staticNodeOutsideNode);
   storage.addNode(dynamicNodeInsideNode);
   storage.addNode(dynamicNodeOutsideNode);

   Array<NodeA*> nodes;
   storage.query(Ray(D3DXVECTOR3(0, 0, 0.5f), D3DXVECTOR3(0, 0, 1)), nodes);

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, nodes.size());
   CPPUNIT_ASSERT_EQUAL(staticNodeInsideQuery, nodes[0]);
   CPPUNIT_ASSERT_EQUAL(dynamicNodeInsideNode, nodes[1]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(TNodesSpatialStorage, narrowPhaseQueries)
{
   TestStorage storage(new SolidTestStorage());
   
   NodeA* node = new NodeA();
   node->addTriangle(new Triangle(D3DXVECTOR3(-3, 1, 0), D3DXVECTOR3(3, 1, 0), D3DXVECTOR3(-3, -1, 0)));
   node->setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 3));
   D3DXMatrixTranslation(&(node->accessLocalMtx()), 0, 0, 10);
   storage.addNode(node);

   // run the tests
   Array<NodeA*> nodes;

   nodes.clear();
   storage.detailedQuery(Ray(D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, nodes.size());
   CPPUNIT_ASSERT_EQUAL(node, nodes[0]);

   nodes.clear();
   storage.detailedQuery(Ray(D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());

   // the object has been  lifted a bit so that its bounding volume will still
   // intersect the ray, but its silhouette won't - now check to see if this
   // transformation change gets included in the query results
   D3DXMatrixTranslation(&(node->accessLocalMtx()), 0, 1, 10);
   nodes.clear();
   storage.detailedQuery(Ray(D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(TNodesSpatialStorage, erroneousCase)
{
   TestStorage storage(new SolidTestStorage());

   // create the node we'll use for rendering
   NodeA* node = new NodeA();
   node->addTriangle(new Triangle(D3DXVECTOR3(0, 4, 0), D3DXVECTOR3(4, 0, 0), D3DXVECTOR3(-4, 0, 0)));
   node->setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 20));
   D3DXMatrixTranslation(&(node->accessLocalMtx()), 0, 0, 1);
   storage.addNode(node);


   // run the tests
   Array<NodeA*> nodes;
   D3DXVECTOR3 rayDir(0, -0.4f, 0.9f);
   storage.detailedQuery(Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -0.4f, 0.9f)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());

   nodes.clear();
   storage.detailedQuery(Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.3f, 0, 0.95f)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());
}

///////////////////////////////////////////////////////////////////////////////
