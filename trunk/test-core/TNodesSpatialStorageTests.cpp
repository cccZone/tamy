#include "core-TestFramework\TestFramework.h"
#include "NodeA.h"
#include "NodeB.h"
#include "core\TNodesSpatialStorage.h"
#include "core\LinearStorage.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

typedef LinearStorage<NodeA> SolidTestStorage;
typedef TNodesSpatialStorage<NodeA, LinearStorage> TestStorage;

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(TNodesSpatialStorage, addingNodesThatCanBeManagedByLeafManagers)
{
   SolidTestStorage* storage = new SolidTestStorage();
   TestStorage sceneForNodeA(storage);

   NodeA nodeA;
   NodeB nodeB;

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, storage->size());

   sceneForNodeA.addNode(&nodeA);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, storage->size());

   sceneForNodeA.addNode(&nodeB);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, storage->size());

   sceneForNodeA.removeNode(nodeA);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, storage->size());

   sceneForNodeA.removeNode(nodeB);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, storage->size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(TNodesSpatialStorage, addingHierarchyOfNodes)
{
   SolidTestStorage* storage = new SolidTestStorage();
   TestStorage sceneForNodeA(storage);

   Node* head = new Node("head");
   NodeA* headAppearance = new NodeA();
   NodeB* headSound = new NodeB();
   head->addChild(headAppearance);
   head->addChild(headSound);

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, storage->size());

   sceneForNodeA.addNode(head);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, storage->size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(TNodesSpatialStorage, connectingNodesUpdatesManagers)
{
   SolidTestStorage* storage = new SolidTestStorage();
   TestStorage sceneForNodeA(storage);

   Node* someNode = new Node("someNode");
   NodeA* node1 = new NodeA();
   NodeA* node2 = new NodeA();

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, storage->size());

   sceneForNodeA.addNode(someNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, storage->size());

   someNode->addChild(node1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, storage->size());

   someNode->addChild(node2);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, storage->size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(TNodesSpatialStorage, attachingSubhierarchyOfNodes)
{
   SolidTestStorage* storage = new SolidTestStorage();
   TestStorage sceneForNodeA(storage);

   Node* someNode = new Node("someNode");

   NodeA* subhierarchyRoot = new NodeA();
   NodeA* subhierarchyChild = new NodeA();
   subhierarchyRoot->addChild(subhierarchyChild);

   NodeA danglingNode;
   NodeA* anotherDanglingNode = new NodeA();

   sceneForNodeA.addNode(someNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, storage->size());

   someNode->addChild(subhierarchyRoot);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, storage->size());

   // now check if anything we add to the subhierarchy will also be added to the scene manager
   subhierarchyChild->addChild(&danglingNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, storage->size());

   // check that when we detach the dangling node we previoulsy
   // attached, it will get removed from the scene manager
   // as well as from under the scrutiny of the scene manager,
   // meaning that if we attach something to it, that thing WILL NOT
   // end up added to the observing scene manager
   subhierarchyChild->removeChild(danglingNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, storage->size());

   danglingNode.addChild(anotherDanglingNode);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, sceneForNodeA.root().getChildrenCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, storage->size());
}


///////////////////////////////////////////////////////////////////////////////
