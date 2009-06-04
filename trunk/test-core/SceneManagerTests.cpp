#include "core-TestFramework\TestFramework.h"
#include "core\CompositeSceneManager.h"
#include "NodeA.h"
#include "NodeB.h"
#include "SceneManagerMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST(CompositeSceneManager, addingNodesThatCanBeManagedByLeafManagers)
{
   CompositeSceneManager fullScene;
   SceneManagerMock<NodeA>* sceneForNodeA = new SceneManagerMock<NodeA>();
   SceneManagerMock<NodeB>* sceneForNodeB = new SceneManagerMock<NodeB>();

   fullScene.addSceneManager(sceneForNodeA);
   fullScene.addSceneManager(sceneForNodeB);

   NodeA nodeA;
   NodeB nodeB;

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA->getNodesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeB->getNodesCount());

   fullScene.addNode(&nodeA);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA->getNodesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeB->getNodesCount());

   fullScene.addNode(&nodeB);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA->getNodesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeB->getNodesCount());

   fullScene.removeNode(nodeA);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA->getNodesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeB->getNodesCount());

   fullScene.removeNode(nodeB);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA->getNodesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeB->getNodesCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(CompositeSceneManager, addingHierarchyOfNodes)
{
   CompositeSceneManager fullScene;
   SceneManagerMock<NodeA>* sceneForNodeA = new SceneManagerMock<NodeA>();
   SceneManagerMock<NodeB>* sceneForNodeB = new SceneManagerMock<NodeB>();

   fullScene.addSceneManager(sceneForNodeA);
   fullScene.addSceneManager(sceneForNodeB);

   Node* head = new Node("head", false);
   NodeA* headAppearance = new NodeA();
   NodeB* headSound = new NodeB();
   head->addChild(headAppearance);
   head->addChild(headSound);

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA->getNodesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeB->getNodesCount());

   fullScene.addNode(head);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA->getNodesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeB->getNodesCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(SceneManager, connectingNodesUpdatesManagers)
{
   CompositeSceneManager fullScene;
   SceneManagerMock<NodeA>* sceneForNodeA = new SceneManagerMock<NodeA>();
   SceneManagerMock<NodeB>* sceneForNodeB = new SceneManagerMock<NodeB>();

   fullScene.addSceneManager(sceneForNodeA);
   fullScene.addSceneManager(sceneForNodeB);

   Node* someNode = new Node("someNode", false);
   NodeA* nodeA = new NodeA();
   NodeB* nodeB = new NodeB();

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA->getNodesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeB->getNodesCount());

   fullScene.addNode(someNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA->getNodesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeB->getNodesCount());

   someNode->addChild(nodeA);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA->getNodesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeB->getNodesCount());

   someNode->addChild(nodeB);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA->getNodesCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeB->getNodesCount());
}

///////////////////////////////////////////////////////////////////////////////
