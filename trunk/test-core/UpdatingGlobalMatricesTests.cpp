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

TEST(UpdatingGlobalMatrices, parentMatrixChangesTwoLevelsAboveThecheckedNode)
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

