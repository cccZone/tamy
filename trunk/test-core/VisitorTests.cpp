#include "core-TestFramework\TestFramework.h"
#include "core\Node.h"
#include "NodeVisitorMock.h"
#include "SpecializedNodeVisitorMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST(NodeVisitor, visitingSingleNode)
{
   NodeStub root;
   NodeVisitorMock visitor;

   root.accept(visitor);

   CPPUNIT_ASSERT_EQUAL(1, visitor.getVisitedNodesCount());
};

///////////////////////////////////////////////////////////////////////////////

TEST(NodeVisitor, visitingNodesHierarchy)
{
   NodeStub root;
   NodeStub* child = new NodeStub();
   root.addChild(child);

   NodeVisitorMock visitor;

   root.accept(visitor);

   CPPUNIT_ASSERT_EQUAL(2, visitor.getVisitedNodesCount());
};

///////////////////////////////////////////////////////////////////////////////

TEST(NodeVisitor, visitingSpecializedNodesHierarchy)
{
   Node root;
   Node* child = new Node();
   SpecializedNodeStub* specializedNode = new SpecializedNodeStub();
   root.addChild(child);
   root.addChild(specializedNode);

   SpecializedNodeVisitorMock visitor;

   root.accept(visitor);

   CPPUNIT_ASSERT_EQUAL(1, visitor.getVisitedNodesCount());
};

///////////////////////////////////////////////////////////////////////////////
