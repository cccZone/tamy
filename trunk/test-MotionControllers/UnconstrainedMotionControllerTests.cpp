#include "TestFramework.h"
#include "UnconstrainedMotionController.h"
#include "Node.h"
#include "MatrixWriter.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

TEST(UnconstrainedMotionController, settingPosition)
{
   Node root;
   Node* node = new Node();
   root.addChild(node);
   
   // let's translate the parent node, so that we can check the differences
   // between the node's local and global matrix
   D3DXMATRIX rootLocalMtx = root.getLocalMtx();
   rootLocalMtx._41 = 10;
   root.setLocalMtx(rootLocalMtx);

   D3DXMATRIX identityMtx;
   D3DXMatrixIdentity(&identityMtx);

   // at it's original position, since the node's local matrix is an identity matrix,
   // it's at the origin of it's parent local coordinate system
   CPPUNIT_ASSERT_EQUAL(identityMtx, node->getLocalMtx());
   CPPUNIT_ASSERT_EQUAL(root.getLocalMtx(), node->getGlobalMtx());

   // let's put the controller into play...
   UnconstrainedMotionController controller(*node);
   D3DXVECTOR3 newGlobalPos(10, 20, 30);
   controller.setPosition(newGlobalPos);

   // parent's matrix shouldn't change
   CPPUNIT_ASSERT_EQUAL(rootLocalMtx, root.getLocalMtx());

   // the node's global matrix should now point to the position we just set
   D3DXMATRIX globalPosMtx;
   D3DXMatrixIdentity(&globalPosMtx);
   globalPosMtx._41 = newGlobalPos.x; globalPosMtx._42 = newGlobalPos.y; globalPosMtx._43 = newGlobalPos.z;
   CPPUNIT_ASSERT_EQUAL(globalPosMtx, node->getGlobalMtx());

   // and the node's local matrix should be a a transformation
   // between the parent's local mtx and the child's global mtx
   D3DXMATRIX localPosMtx;
   D3DXMatrixIdentity(&localPosMtx);
   localPosMtx._41 = 0; localPosMtx._42 = 20; localPosMtx._43 = 30;
   CPPUNIT_ASSERT_EQUAL(localPosMtx, node->getLocalMtx());
}

///////////////////////////////////////////////////////////////////////////////

TEST(UnconstrainedMotionController, moving)
{
   Node root;
   Node* node = new Node();
   root.addChild(node);
   
   // let's translate the parent node, so that we can check the differences
   // between the node's local and global matrix
   D3DXMATRIX rootLocalMtx = root.getLocalMtx();
   rootLocalMtx._41 = 10;
   root.setLocalMtx(rootLocalMtx);

   D3DXMATRIX identityMtx;
   D3DXMatrixIdentity(&identityMtx);

   // at it's original position, since the node's local matrix is an identity matrix,
   // it's at the origin of it's parent local coordinate system
   CPPUNIT_ASSERT_EQUAL(identityMtx, node->getLocalMtx());
   CPPUNIT_ASSERT_EQUAL(root.getLocalMtx(), node->getGlobalMtx());

   // let's put the controller into play...
   UnconstrainedMotionController controller(*node);
   D3DXVECTOR3 translationVec(10, 20, 30);
   controller.move(translationVec);

   // the node's global matrix should now point to the position we just set
   D3DXMATRIX globalMtx = rootLocalMtx;
   globalMtx._41 += translationVec.x; globalMtx._42 = translationVec.y; globalMtx._43 = translationVec.z;
   CPPUNIT_ASSERT_EQUAL(globalMtx, node->getGlobalMtx());
}

///////////////////////////////////////////////////////////////////////////////

TEST(UnconstrainedMotionController, changingPitch)
{
   Node node;

   // let's put the controller into play...
   UnconstrainedMotionController controller(node);
   controller.rotate(-90, 0, 0); // let's change the pitch by 90 degrees

   // the node's global matrix should now point to the position we just set
   D3DXMATRIX expectedGlobalMtx;
   D3DXMatrixIdentity(&expectedGlobalMtx);
   expectedGlobalMtx._21 = 0; expectedGlobalMtx._22 = 0; expectedGlobalMtx._23 = -1;
   expectedGlobalMtx._31 = 0; expectedGlobalMtx._32 = 1; expectedGlobalMtx._33 = 0;

   D3DXMATRIX actualGlobalMtx = node.getGlobalMtx();
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
   Node node;

   // let's put the controller into play...
   UnconstrainedMotionController controller(node);
   controller.rotate(0, 90, 0); // let's change the yaw by 90 degrees

   // the node's global matrix should now point to the position we just set
   D3DXMATRIX expectedGlobalMtx;
   D3DXMatrixIdentity(&expectedGlobalMtx);
   expectedGlobalMtx._11 = 0; expectedGlobalMtx._12 = 0; expectedGlobalMtx._13 = -1;
   expectedGlobalMtx._31 = 1; expectedGlobalMtx._32 = 0; expectedGlobalMtx._33 = 0;

   D3DXMATRIX actualGlobalMtx = node.getGlobalMtx();
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
   Node node;

   // let's put the controller into play...
   UnconstrainedMotionController controller(node);
   controller.rotate(0, 0, 90); // let's change the roll by 90 degrees

   // the node's global matrix should now point to the position we just set
   D3DXMATRIX expectedGlobalMtx;
   D3DXMatrixIdentity(&expectedGlobalMtx);
   expectedGlobalMtx._11 = 0; expectedGlobalMtx._12 = 1; expectedGlobalMtx._13 = 0;
   expectedGlobalMtx._21 = -1; expectedGlobalMtx._22 = 0; expectedGlobalMtx._23 = 0;

   D3DXMATRIX actualGlobalMtx = node.getGlobalMtx();
   for (int col = 0; col < 4; col++)
   {
      for (int row = 0; row < 4; row++)
      {
         CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGlobalMtx.m[col][row], actualGlobalMtx.m[col][row], 0.01);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
