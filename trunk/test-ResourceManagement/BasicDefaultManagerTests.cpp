#include "core-TestFramework\TestFramework.h"
#include "core-ResourceManagement\BasicSceneManager.h"
#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

TEST(BasicSceneManagerTests, addingNodesToScene)
{
   BasicSceneManager sceneManager;
   Node* newNode = new Node();
   Node* anotherNewNode = new Node();

   sceneManager.addNode(newNode);
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (1), sceneManager.getRootNode().getChildrenCount());

   sceneManager.addNode(anotherNewNode);
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (2), sceneManager.getRootNode().getChildrenCount());
}

///////////////////////////////////////////////////////////////////////////////
