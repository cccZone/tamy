#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\Node.h"
#include "core\Delegate.h"
#include "core\NodeActionsExecutor.h"
#include "NodeActionMock.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

TEST(NodeActionsExecutor, registeringActionForNode)
{
   Node node("node", false);
   NodeActionMock nodeAction;

   NodeActionsExecutor executor;
   executor.add(node, NodeActionDelegate::FROM_METHOD(NodeActionMock, execute, &nodeAction));

   CPPUNIT_ASSERT_EQUAL(0, nodeAction.getExecutionsCount());

   executor.execute(node);
   CPPUNIT_ASSERT_EQUAL(1, nodeAction.getExecutionsCount());
};

///////////////////////////////////////////////////////////////////////////////

TEST(NodeActionsExecutor, executingMultipleActionsOneByOne)
{
   Node node1("node1", false);
   Node node2("node2", false);
   NodeActionMock action1;
   NodeActionMock action2;

   NodeActionsExecutor executor;
   executor.add(node1, NodeActionDelegate::FROM_METHOD(NodeActionMock, execute, &action1));
   executor.add(node2, NodeActionDelegate::FROM_METHOD(NodeActionMock, execute, &action2));

   CPPUNIT_ASSERT_EQUAL(0, action1.getExecutionsCount());
   CPPUNIT_ASSERT_EQUAL(0, action2.getExecutionsCount());

   executor.execute(node1);
   CPPUNIT_ASSERT_EQUAL(1,  action1.getExecutionsCount());
   CPPUNIT_ASSERT_EQUAL(0, action2.getExecutionsCount());

   executor.execute(node2);
   CPPUNIT_ASSERT_EQUAL(0, action1.getExecutionsCount());
   CPPUNIT_ASSERT_EQUAL(1,  action2.getExecutionsCount());
};

///////////////////////////////////////////////////////////////////////////////

TEST(NodeActionsExecutor, executingMultipleActionsSimultaneously)
{
   Node node1("node1", false);
   Node node2("node2", false);
   NodeActionMock action1;
   NodeActionMock action2;

   NodeActionsExecutor executor;
   executor.add(node1, NodeActionDelegate::FROM_METHOD(NodeActionMock, execute, &action1));
   executor.add(node2, NodeActionDelegate::FROM_METHOD(NodeActionMock, execute, &action2));

   CPPUNIT_ASSERT_EQUAL(0, action1.getExecutionsCount());
   CPPUNIT_ASSERT_EQUAL(0, action2.getExecutionsCount());

   Array<Node*> nodes;
   nodes.push_back(&node1);
   nodes.push_back(&node2);
   executor.execute(nodes);
   CPPUNIT_ASSERT_EQUAL(1, action1.getExecutionsCount());
   CPPUNIT_ASSERT_EQUAL(1, action2.getExecutionsCount());
};

///////////////////////////////////////////////////////////////////////////////

TEST(NodeActionsExecutor, executingActionOfParentNodeThroughChildNode)
{
   Node root("root", false);
   Node* child = new Node("child", false);
   root.addChild(child);

   NodeActionMock action;

   NodeActionsExecutor executor;
   executor.add(root, NodeActionDelegate::FROM_METHOD(NodeActionMock, execute, &action));

   CPPUNIT_ASSERT_EQUAL(0, action.getExecutionsCount());

   executor.execute(*child);
   CPPUNIT_ASSERT_EQUAL(1, action.getExecutionsCount());
};

///////////////////////////////////////////////////////////////////////////////

TEST(NodeActionsExecutor, actionsOnTreeExecutedOnlyOnceWhenHierarchyPassed)
{
   Node root("root", false);
   Node* child1 = new Node("child1", false);
   Node* child2 = new Node("child2", false);
   root.addChild(child1);
   root.addChild(child2);

   NodeActionMock action;

   NodeActionsExecutor executor;
   executor.add(root, NodeActionDelegate::FROM_METHOD(NodeActionMock, execute, &action));

   CPPUNIT_ASSERT_EQUAL(0, action.getExecutionsCount());

   Array<Node*> nodes;
   nodes.push_back(child1);
   nodes.push_back(child2);
   executor.execute(nodes);
   CPPUNIT_ASSERT_EQUAL(1, action.getExecutionsCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(NodeActionsExecutor, nodesFromDifferentHierarchyTrees)
{
   Node root1("root1", false);
   Node* child1 = new Node("child1", false);
   Node* child2 = new Node("child2", false);
   root1.addChild(child1);
   root1.addChild(child2);

   Node root2("root2", false);
   Node* child3 = new Node("child3", false);
   Node* child4 = new Node("child4", false);
   root2.addChild(child3);
   root2.addChild(child4);

   NodeActionMock action1;
   NodeActionMock action2;

   NodeActionsExecutor executor;
   executor.add(root1, NodeActionDelegate::FROM_METHOD(NodeActionMock, execute, &action1));
   executor.add(root2, NodeActionDelegate::FROM_METHOD(NodeActionMock, execute, &action2));

   CPPUNIT_ASSERT_EQUAL(0, action1.getExecutionsCount());
   CPPUNIT_ASSERT_EQUAL(0, action2.getExecutionsCount());

   Array<Node*> nodes;
   nodes.push_back(child1);
   nodes.push_back(child2);
   nodes.push_back(child3);
   nodes.push_back(child4);
   executor.execute(nodes);
   CPPUNIT_ASSERT_EQUAL(1, action1.getExecutionsCount());
   CPPUNIT_ASSERT_EQUAL(1, action2.getExecutionsCount());
}

///////////////////////////////////////////////////////////////////////////////
