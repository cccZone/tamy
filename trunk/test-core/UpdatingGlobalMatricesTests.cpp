#include "core-TestFramework\TestFramework.h"
#include <d3dx9.h>
#include "core\Node.h"
#include "core\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(UpdatingGlobalMatrices, singleNode)
{
   Node root("root", false);
   
   D3DXMATRIX matrix;
   D3DXMatrixIdentity(&matrix);
   D3DXMatrixTranslation(&matrix, 5, 0, 0);

   root.setLocalMtx(matrix);
   CPPUNIT_ASSERT_EQUAL(matrix, root.getLocalMtx());
   CPPUNIT_ASSERT_EQUAL(matrix, root.getGlobalMtx());
};

///////////////////////////////////////////////////////////////////////////////

TEST(UpdatingGlobalMatrices, simpleHierarchy)
{
   Node root("root", false);
   Node* child = new Node("child", false);
   root.addChild(child);
   
   D3DXMATRIX childLocalMatrix = child->getLocalMtx();
   D3DXMATRIX matrix;
   D3DXMatrixIdentity(&matrix);
   D3DXMatrixTranslation(&matrix, 5, 0, 0);

   root.setLocalMtx(matrix);
   CPPUNIT_ASSERT_EQUAL(childLocalMatrix, child->getLocalMtx());
   CPPUNIT_ASSERT_EQUAL(matrix, child->getGlobalMtx());
};

///////////////////////////////////////////////////////////////////////////////

TEST(UpdatingGlobalMatrices, parentMatrixChangesTwoLevelsAboveTheCheckedNode)
{
   Node root("root", false);
   Node* childLevel1 = new Node("childLevel1", false);
   Node* childLevel2 = new Node("childLevel2", false);
   root.addChild(childLevel1);
   childLevel1->addChild(childLevel2);
   
   D3DXMATRIX matrix;
   D3DXMatrixIdentity(&matrix);
   D3DXMatrixTranslation(&matrix, 5, 0, 0);

   root.setLocalMtx(matrix);
   CPPUNIT_ASSERT_EQUAL(matrix, childLevel2->getGlobalMtx());
};

///////////////////////////////////////////////////////////////////////////////

TEST(UpdatingGlobalMatrices, sequenceOfUpdatesInfluencingChild)
{
   Node root("root", false);
   Node* child = new Node("child", false);
   root.addChild(child);

   // 1st update
   D3DXMATRIX rootLocalMatrix; D3DXMatrixTranslation(&rootLocalMatrix, 5, 0, 0);
   root.setLocalMtx(rootLocalMatrix);
   CPPUNIT_ASSERT_EQUAL(rootLocalMatrix, child->getGlobalMtx());

   // 2nd update
   D3DXMatrixTranslation(&rootLocalMatrix, 10, 0, 0);
   root.setLocalMtx(rootLocalMatrix);
   CPPUNIT_ASSERT_EQUAL(rootLocalMatrix, child->getGlobalMtx());
};

///////////////////////////////////////////////////////////////////////////////
