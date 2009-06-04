#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\Node.h"
#include "core\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(NodesHierarchy, attachingChild)
{
   Node root("root", false);
   Node* child = new Node("child", false);
   
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
   Node root("root", false);
   Node differentRoot("differentRoot", false);
   Node* child = new Node("child", false);
   
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

TEST(NodesHierarchy, dynamicFlagInheritedBySubtree)
{
   Node root("root", true);
   Node* child = new Node("child", false);
   Node* lowerChild = new Node("lowerChild", true);

   CPPUNIT_ASSERT_EQUAL(true, root.isDynamic());
   CPPUNIT_ASSERT_EQUAL(false, child->isDynamic());
   CPPUNIT_ASSERT_EQUAL(true, lowerChild->isDynamic());

   child->addChild(lowerChild);
   CPPUNIT_ASSERT_EQUAL(true, root.isDynamic());
   CPPUNIT_ASSERT_EQUAL(false, child->isDynamic());
   CPPUNIT_ASSERT_EQUAL(false, lowerChild->isDynamic());

   root.addChild(child);
   CPPUNIT_ASSERT_EQUAL(true, root.isDynamic());
   CPPUNIT_ASSERT_EQUAL(true, child->isDynamic());
   CPPUNIT_ASSERT_EQUAL(true, lowerChild->isDynamic());

   root.removeChild(*child);
   CPPUNIT_ASSERT_EQUAL(true, root.isDynamic());
   CPPUNIT_ASSERT_EQUAL(false, child->isDynamic());
   CPPUNIT_ASSERT_EQUAL(false, lowerChild->isDynamic());

   child->removeChild(*lowerChild);
   CPPUNIT_ASSERT_EQUAL(true, root.isDynamic());
   CPPUNIT_ASSERT_EQUAL(false, child->isDynamic());
   CPPUNIT_ASSERT_EQUAL(true, lowerChild->isDynamic());

   delete child;
   delete lowerChild;
}

///////////////////////////////////////////////////////////////////////////////
