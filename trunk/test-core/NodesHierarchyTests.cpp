#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\Node.h"
#include "core\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(NodesHierarchy, attachingChild)
{
   Node root;
   Node* child = new Node();
   
   CPPUNIT_ASSERT_EQUAL(false, root.hasParent());
   CPPUNIT_ASSERT_EQUAL(false, child->hasParent());

   root.addChild(child);

   CPPUNIT_ASSERT_EQUAL(false, root.hasParent());
   CPPUNIT_ASSERT_EQUAL(true, child->hasParent());
   CPPUNIT_ASSERT_EQUAL(&root, &(child->getParent()));
};

///////////////////////////////////////////////////////////////////////////////

TEST(NodesHierarchy, switchingChildren)
{
   Node root;
   Node differentRoot;
   Node* child = new Node();
   
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (0), root.getChildrenCount());
   CPPUNIT_ASSERT_EQUAL(false, differentRoot.hasParent());
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (0), differentRoot.getChildrenCount());

   root.addChild(child);

   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (1), root.getChildrenCount());
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (0), differentRoot.getChildrenCount());

   CPPUNIT_ASSERT_EQUAL(true, child->hasParent());
   CPPUNIT_ASSERT_EQUAL(&root, &(child->getParent()));

   differentRoot.addChild(child);

   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (0), root.getChildrenCount());
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (1), differentRoot.getChildrenCount());

   CPPUNIT_ASSERT_EQUAL(true, child->hasParent());
   CPPUNIT_ASSERT_EQUAL(&differentRoot, &(child->getParent()));
};

///////////////////////////////////////////////////////////////////////////////
