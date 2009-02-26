#include "TestFramework.h"
#include <d3dx9.h>
#include "Node.h"
#include "MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(UpdatingGlobalMatrices, singleNode)
{
   Node root;
   
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
   Node root;
   Node* child = new Node();
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
   Node root;
   Node* childLevel1 = new Node();
   Node* childLevel2 = new Node();
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
   Node root;
   Node* child = new Node();
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
