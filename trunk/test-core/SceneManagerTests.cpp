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

TEST(SceneManager, addingUnknownNodeType)
{
   CompositeSceneManager fullScene;
   SceneManagerMock<NodeA>* sceneForNodeA = new SceneManagerMock<NodeA>();
   fullScene.addSceneManager(sceneForNodeA);

   Node* someNode = new Node("someNode", false);
   NodeB unknownNode;
   NodeA* knownNode1 = new NodeA();
   NodeA* knownNode2 = new NodeA();

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA->getNodesCount());

   fullScene.addNode(someNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA->getNodesCount());

   someNode->addChild(&unknownNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA->getNodesCount());

   unknownNode.addChild(knownNode1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA->getNodesCount());

   unknownNode.addChild(knownNode2);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, sceneForNodeA->getNodesCount());

   someNode->removeChild(unknownNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA->getNodesCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(SceneManager, attachingSubhierarchyOfNodes)
{
   CompositeSceneManager fullScene;
   SceneManagerMock<NodeA>* sceneForNodeA = new SceneManagerMock<NodeA>();
   fullScene.addSceneManager(sceneForNodeA);

   Node* someNode = new Node("someNode", false);

   NodeA* subhierarchyRoot = new NodeA();
   NodeA* subhierarchyChild = new NodeA();
   subhierarchyRoot->addChild(subhierarchyChild);

   NodeA danglingNode;
   NodeA* anotherDanglingNode = new NodeA();

   fullScene.addNode(someNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA->getNodesCount());

   someNode->addChild(subhierarchyRoot);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, sceneForNodeA->getNodesCount());

   // now check if anything we add to the subhierarchy will also be added to the scene manager
   subhierarchyChild->addChild(&danglingNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, sceneForNodeA->getNodesCount());

   // check that when we detach the dangling node we previoulsy
   // attached, it will get removed from the scene manager
   // as well as from under the scrutiny of the scene manager,
   // meaning that if we attach something to it, that thing WILL NOT
   // end up added to the observing scene manager
   subhierarchyChild->removeChild(danglingNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, sceneForNodeA->getNodesCount());

   danglingNode.addChild(anotherDanglingNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, fullScene.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, sceneForNodeA->getNodesCount());
}


///////////////////////////////////////////////////////////////////////////////
