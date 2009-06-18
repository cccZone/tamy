#include "core-TestFramework\TestFramework.h"
#include "core\CompositeSceneManager.h"
#include "NodeA.h"
#include "SceneManagerMock.h"
#include "core-Renderer\Camera.h"
#include "core\Triangle.h"
#include "core\MatrixWriter.h"
#include "core\Ray.h"
#include "core\Array.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

TEST(SceneAspectManager, broadPhaseQueries)
{
   SceneManagerMock<NodeA> sceneManager;

   NodeA staticNodeInsideQuery(false);
   NodeA staticNodeOutsideNode(false);
   NodeA dynamicNodeInsideNode(true);
   NodeA dynamicNodeOutsideNode(true);

   staticNodeInsideQuery.setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 1));
   staticNodeOutsideNode.setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 1));
   dynamicNodeInsideNode.setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 1));
   dynamicNodeOutsideNode.setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 1));
   D3DXMatrixTranslation(&(staticNodeOutsideNode.accessLocalMtx()), 0, 0, -10);
   D3DXMatrixTranslation(&(dynamicNodeOutsideNode.accessLocalMtx()), 0, 0, -10);

   sceneManager.addNode(&staticNodeInsideQuery);
   sceneManager.addNode(&staticNodeOutsideNode);
   sceneManager.addNode(&dynamicNodeInsideNode);
   sceneManager.addNode(&dynamicNodeOutsideNode);

   Array<NodeA*> nodes;
   sceneManager.query<NodeA>(Ray(D3DXVECTOR3(0, 0, 0.5f), D3DXVECTOR3(0, 0, 1)), nodes);

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, nodes.size());
   CPPUNIT_ASSERT_EQUAL((NodeA*)&staticNodeInsideQuery, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((NodeA*)&dynamicNodeInsideNode, nodes[1]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(SceneAspectManager, narrowPhaseQueries)
{
   SceneManagerMock<NodeA> sceneManager;
   
   NodeA node;
   node.addTriangle(new Triangle(D3DXVECTOR3(-3, 1, 0), D3DXVECTOR3(3, 1, 0), D3DXVECTOR3(-3, -1, 0)));
   node.setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 3));
   D3DXMatrixTranslation(&(node.accessLocalMtx()), 0, 0, 10);
   sceneManager.addNode(&node);

   // run the tests
   Array<NodeA*> nodes;

   nodes.clear();
   sceneManager.detailedQuery<NodeA>(Ray(D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, nodes.size());
   CPPUNIT_ASSERT_EQUAL((NodeA*)&node, nodes[0]);

   nodes.clear();
   sceneManager.detailedQuery<NodeA>(Ray(D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());

   // the object has been  lifted a bit so that its bounding volume will still
   // intersect the ray, but its silhouette won't - now check to see if this
   // transformation change gets included in the query results
   D3DXMatrixTranslation(&(node.accessLocalMtx()), 0, 1, 10);
   nodes.clear();
   sceneManager.detailedQuery<NodeA>(Ray(D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0, 1)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(SceneAspectManager, erroneousCase)
{
   SceneManagerMock<NodeA> sceneManager;

   // create the node we'll use for rendering
   NodeA node;
   node.addTriangle(new Triangle(D3DXVECTOR3(0, 4, 0), D3DXVECTOR3(4, 0, 0), D3DXVECTOR3(-4, 0, 0)));
   node.setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 20));
   D3DXMatrixTranslation(&(node.accessLocalMtx()), 0, 0, 1);
   sceneManager.addNode(&node);


   // run the tests
   Array<NodeA*> nodes;
   D3DXVECTOR3 rayDir(0, -0.4f, 0.9f);
   sceneManager.detailedQuery<NodeA>(Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -0.4f, 0.9f)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());

   nodes.clear();
   sceneManager.detailedQuery<NodeA>(Ray(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.3f, 0, 0.95f)), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, nodes.size());
}

///////////////////////////////////////////////////////////////////////////////
