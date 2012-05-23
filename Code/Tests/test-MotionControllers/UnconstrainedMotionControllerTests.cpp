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
   rootLocalMtx.m41 = 10;
   root.setLocalMtx(rootLocalMtx);

   Matrix identityMtx = Matrix::IDENTITY;

   // at it's original position, since the node's local matrix is an identity matrix,
   // it's at the origin of it's parent local coordinate system
   CPPUNIT_ASSERT_EQUAL(identityMtx, node->getLocalMtx());
   CPPUNIT_ASSERT_EQUAL(root.getLocalMtx(), node->getGlobalMtx());

   // let's put the controller into play...
   UnconstrainedMotionController controller(*node);
   Vector newGlobalPos(10, 20, 30);
   controller.setPosition(newGlobalPos);

   // parent's matrix shouldn't change
   CPPUNIT_ASSERT_EQUAL(rootLocalMtx, root.getLocalMtx());

   // the node's global matrix should now point to the position we just set
   Matrix globalPosMtx = Matrix::IDENTITY;
   globalPosMtx.m41 = newGlobalPos.x; globalPosMtx.m42 = newGlobalPos.y; globalPosMtx.m43 = newGlobalPos.z;
   CPPUNIT_ASSERT_EQUAL(globalPosMtx, node->getGlobalMtx());

   // and the node's local matrix should be a a transformation
   // between the parent's local mtx and the child's global mtx
   Matrix localPosMtx = Matrix::IDENTITY;
   localPosMtx.m41 = 0; localPosMtx.m42 = 20; localPosMtx.m43 = 30;
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
   rootLocalMtx.m41 = 10;
   root.setLocalMtx(rootLocalMtx);

   Matrix identityMtx = Matrix::IDENTITY;

   // at it's original position, since the node's local matrix is an identity matrix,
   // it's at the origin of it's parent local coordinate system
   CPPUNIT_ASSERT_EQUAL(identityMtx, node->getLocalMtx());
   CPPUNIT_ASSERT_EQUAL(root.getLocalMtx(), node->getGlobalMtx());

   // let's put the controller into play...
   UnconstrainedMotionController controller(*node);
   Vector translationVec(10, 20, 30);
   controller.move(translationVec);

   // the node's global matrix should now point to the position we just set
   Matrix globalMtx = rootLocalMtx;
   globalMtx.m41 += translationVec.x; globalMtx.m42 = translationVec.y; globalMtx.m43 = translationVec.z;
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
   Matrix expectedGlobalMtx = Matrix::IDENTITY;
   expectedGlobalMtx.m21 = 0; expectedGlobalMtx.m22 = 0; expectedGlobalMtx.m23 = -1;
   expectedGlobalMtx.m31 = 0; expectedGlobalMtx.m32 = 1; expectedGlobalMtx.m33 = 0;

   Matrix actualGlobalMtx = node.getGlobalMtx();
   for (int col = 0; col < 4; col++)
   {
      for (int row = 0; row < 4; row++)
      {
         CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGlobalMtx.m[col][row], actualGlobalMtx.m[col][row], 0.01);
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
   Matrix expectedGlobalMtx = Matrix::IDENTITY;
   expectedGlobalMtx.m11 = 0; expectedGlobalMtx.m12 = 0; expectedGlobalMtx.m13 = -1;
   expectedGlobalMtx.m31 = 1; expectedGlobalMtx.m32 = 0; expectedGlobalMtx.m33 = 0;

   Matrix actualGlobalMtx = node.getGlobalMtx();
   for (int col = 0; col < 4; col++)
   {
      for (int row = 0; row < 4; row++)
      {
         CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGlobalMtx.m[col][row], actualGlobalMtx.m[col][row], 0.01);
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
   Matrix expectedGlobalMtx = Matrix::IDENTITY;
   expectedGlobalMtx.m11 = 0; expectedGlobalMtx.m12 = 1; expectedGlobalMtx.m13 = 0;
   expectedGlobalMtx.m21 = -1; expectedGlobalMtx.m22 = 0; expectedGlobalMtx.m23 = 0;

   Matrix actualGlobalMtx = node.getGlobalMtx();
   for (int col = 0; col < 4; col++)
   {
      for (int row = 0; row < 4; row++)
      {
         CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGlobalMtx.m[col][row], actualGlobalMtx.m[col][row], 0.01);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
