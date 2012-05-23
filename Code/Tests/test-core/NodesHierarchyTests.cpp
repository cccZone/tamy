#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\Node.h"
#include "core-TestFramework\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(NodesHierarchy, attachingChild)
{
   Node root("root");
   Node* child = new Node("child");
   
   CPPUNIT_ASSERT_EQUAL(false, root.hasParentNode());
   CPPUNIT_ASSERT_EQUAL(false, child->hasParentNode());

   root.addChild(child);

   CPPUNIT_ASSERT_EQUAL(false, root.hasParentNode());
   CPPUNIT_ASSERT_EQUAL(true, child->hasParentNode());
   CPPUNIT_ASSERT_EQUAL(&root, &(child->getParentNode()));
};

///////////////////////////////////////////////////////////////////////////////

TEST(NodesHierarchy, switchingChildren)
{
   Node root("root");
   Node differentRoot("differentRoot");
   Node* child = new Node("child");

   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (0), root.getChildrenCount());
   CPPUNIT_ASSERT_EQUAL(false, differentRoot.hasParentNode());
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (0), differentRoot.getChildrenCount());

   root.addChild(child);

   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (1), root.getChildrenCount());
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (0), differentRoot.getChildrenCount());

   CPPUNIT_ASSERT_EQUAL(true, child->hasParentNode());
   CPPUNIT_ASSERT_EQUAL(&root, &(child->getParentNode()));

   differentRoot.addChild(child);

   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (0), root.getChildrenCount());
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (1), differentRoot.getChildrenCount());

   CPPUNIT_ASSERT_EQUAL(true, child->hasParentNode());
   CPPUNIT_ASSERT_EQUAL(&differentRoot, &(child->getParentNode()));
};

///////////////////////////////////////////////////////////////////////////////

TEST(NodesHierarchy, findingChildren)
{
   Node root("root");
   Node* children[] = { new Node("child1"), new Node("child2"), new Node("child3"), new Node("child4") };
   root.addChild( children[0] );
   root.addChild( children[1] );
   children[0]->addChild( children[2] );
   children[1]->addChild( children[3] );

   CPPUNIT_ASSERT_EQUAL( children[0], root.findNode( "child1" ) );
   CPPUNIT_ASSERT_EQUAL( children[1], root.findNode( "child2" ) );
   CPPUNIT_ASSERT_EQUAL( children[2], root.findNode( "child3" ) );
   CPPUNIT_ASSERT_EQUAL( children[3], root.findNode( "child4" ) );

   CPPUNIT_ASSERT_EQUAL( (Node*)NULL, children[0]->findNode( "child4" ) );
   CPPUNIT_ASSERT_EQUAL( children[3], children[1]->findNode( "child4" ) );

   CPPUNIT_ASSERT_EQUAL( children[2], children[0]->findNode( "child3" ) );
   CPPUNIT_ASSERT_EQUAL( (Node*)NULL, children[1]->findNode( "child3" ) );
};

///////////////////////////////////////////////////////////////////////////////
