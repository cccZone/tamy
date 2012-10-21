#include "core-TestFramework\TestFramework.h"
#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core\Node.h"
#include "core\Vector.h"
#include "core\Matrix.h"


///////////////////////////////////////////////////////////////////////////////

TEST(UnconstrainedMotionController, settingPosition)
{
   Node root("root");
   Node* node = new Node("node");
   root.addChild(node);
   
   // let's translate the parent node, so that we can check the differences
   // between the node's local and global matrix
   Matrix rootLocalMtx = root.getLocalMtx();
   rootLocalMtx( 3, 0 ) = 10;
   root.setLocalMtx(rootLocalMtx);

   Matrix identityMtx; identityMtx.setIdentity();

   // at it's original position, since the node's local matrix is an identity matrix,
   // it's at the origin of it's parent local coordinate system
   CPPUNIT_ASSERT_EQUAL(identityMtx, node->getLocalMtx());
   CPPUNIT_ASSERT_EQUAL(root.getLocalMtx(), node->getGlobalMtx());

   // let's put the controller into play...
   UnconstrainedMotionController controller(*node);
   Vector newGlobalPos; newGlobalPos.set(10, 20, 30);
   controller.setPosition(newGlobalPos);

   // parent's matrix shouldn't change
   CPPUNIT_ASSERT_EQUAL(rootLocalMtx, root.getLocalMtx());

   // the node's global matrix should now point to the position we just set
   Matrix globalPosMtx; globalPosMtx.setIdentity();
   globalPosMtx( 3, 0 ) = newGlobalPos[0]; globalPosMtx( 3, 1 ) = newGlobalPos[1]; globalPosMtx( 3, 2 ) = newGlobalPos[2];
   CPPUNIT_ASSERT_EQUAL( globalPosMtx, node->getGlobalMtx() );

   // and the node's local matrix should be a a transformation
   // between the parent's local mtx and the child's global mtx
   Matrix localPosMtx; localPosMtx.setIdentity();
   localPosMtx( 3, 0 ) = 0; localPosMtx( 3, 1 ) = 20; localPosMtx( 3, 2 ) = 30;
   CPPUNIT_ASSERT_EQUAL(localPosMtx, node->getLocalMtx());
}

///////////////////////////////////////////////////////////////////////////////

TEST(UnconstrainedMotionController, moving)
{
   Node root("root");
   Node* node = new Node("node");
   root.addChild(node);
   
   // let's translate the parent node, so that we can check the differences
   // between the node's local and global matrix
   Matrix rootLocalMtx = root.getLocalMtx();
   rootLocalMtx( 3, 0 ) = 10;
   root.setLocalMtx(rootLocalMtx);

   Matrix identityMtx; identityMtx.setIdentity();

   // at it's original position, since the node's local matrix is an identity matrix,
   // it's at the origin of it's parent local coordinate system
   CPPUNIT_ASSERT_EQUAL(identityMtx, node->getLocalMtx());
   CPPUNIT_ASSERT_EQUAL(root.getLocalMtx(), node->getGlobalMtx());

   // let's put the controller into play...
   UnconstrainedMotionController controller(*node);
   Vector translationVec; translationVec.set(10, 20, 30);
   controller.move(translationVec);

   // the node's global matrix should now point to the position we just set
   Matrix globalMtx = rootLocalMtx;
   globalMtx( 3, 0 ) += translationVec[0]; globalMtx( 3, 1 ) = translationVec[1]; globalMtx( 3, 2 ) = translationVec[2];
   CPPUNIT_ASSERT_EQUAL(globalMtx, node->getGlobalMtx());
}

///////////////////////////////////////////////////////////////////////////////

TEST(UnconstrainedMotionController, changingPitch)
{
   Node node("node");

   // let's put the controller into play...
   UnconstrainedMotionController controller(node);
   controller.rotate(-90, 0, 0); // let's change the pitch by 90 degrees

   // the node's global matrix should now point to the position we just set
   Matrix expectedGlobalMtx; expectedGlobalMtx.setIdentity();
   expectedGlobalMtx( 1, 0 ) = 0; expectedGlobalMtx( 1, 1 ) = 0; expectedGlobalMtx( 1, 2 ) = -1;
   expectedGlobalMtx( 2, 0 ) = 0; expectedGlobalMtx( 2, 1 ) = 1; expectedGlobalMtx( 2, 2 ) = 0;

   Matrix actualGlobalMtx = node.getGlobalMtx();
   for (int col = 0; col < 4; col++)
   {
      for (int row = 0; row < 4; row++)
      {
         CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGlobalMtx( col, row ), actualGlobalMtx( col, row ), 0.01);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST(UnconstrainedMotionController, changingYaw)
{
   Node node("node");

   // let's put the controller into play...
   UnconstrainedMotionController controller(node);
   controller.rotate(0, 90, 0); // let's change the yaw by 90 degrees

   // the node's global matrix should now point to the position we just set
   Matrix expectedGlobalMtx; expectedGlobalMtx.setIdentity();
   expectedGlobalMtx( 0, 0 ) = 0; expectedGlobalMtx( 0, 1 ) = 0; expectedGlobalMtx( 0, 2 ) = -1;
   expectedGlobalMtx( 2, 0 ) = 1; expectedGlobalMtx( 2, 1 ) = 0; expectedGlobalMtx( 2, 2 ) = 0;

   Matrix actualGlobalMtx = node.getGlobalMtx();
   for (int col = 0; col < 4; col++)
   {
      for (int row = 0; row < 4; row++)
      {
         CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGlobalMtx( col, row ), actualGlobalMtx( col, row ), 0.01);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST(UnconstrainedMotionController, changingRoll)
{
   Node node("node");

   // let's put the controller into play...
   UnconstrainedMotionController controller(node);
   controller.rotate(0, 0, 90); // let's change the roll by 90 degrees

   // the node's global matrix should now point to the position we just set
   Matrix expectedGlobalMtx; expectedGlobalMtx.setIdentity();
   expectedGlobalMtx( 0, 0 ) = 0; expectedGlobalMtx( 0, 1 ) = 1; expectedGlobalMtx( 0, 2 ) = 0;
   expectedGlobalMtx( 1, 0 ) = -1; expectedGlobalMtx( 1, 1 ) = 0; expectedGlobalMtx( 1, 2 ) = 0;

   Matrix actualGlobalMtx = node.getGlobalMtx();
   for (int col = 0; col < 4; col++)
   {
      for (int row = 0; row < 4; row++)
      {
         CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGlobalMtx( col, row ), actualGlobalMtx( col, row ), 0.01);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
