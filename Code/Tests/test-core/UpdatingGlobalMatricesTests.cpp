#include "core-TestFramework\TestFramework.h"
#include "core\Node.h"
#include "core-TestFramework\MatrixWriter.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

TEST(UpdatingGlobalMatrices, singleNode)
{
   Node root("root");
   
   Matrix matrix;
   matrix.setTranslation( Vector( 5, 0, 0 ) );

   root.setLocalMtx(matrix);
   CPPUNIT_ASSERT_EQUAL(matrix, root.getLocalMtx());
   CPPUNIT_ASSERT_EQUAL(matrix, root.getGlobalMtx());
};

///////////////////////////////////////////////////////////////////////////////

TEST(UpdatingGlobalMatrices, simpleHierarchy)
{
   Node root("root");
   Node* child = new Node("child");
   root.addChild(child);
   
   Matrix childLocalMatrix = child->getLocalMtx();
   Matrix matrix;
   matrix.setTranslation( Vector( 5, 0, 0 ) );

   root.setLocalMtx(matrix);
   CPPUNIT_ASSERT_EQUAL(childLocalMatrix, child->getLocalMtx());
   CPPUNIT_ASSERT_EQUAL(matrix, child->getGlobalMtx());
};

///////////////////////////////////////////////////////////////////////////////

TEST(UpdatingGlobalMatrices, parentMatrixChangesTwoLevelsAboveTheCheckedNode)
{
   Node root("root");
   Node* childLevel1 = new Node("childLevel1");
   Node* childLevel2 = new Node("childLevel2");
   root.addChild(childLevel1);
   childLevel1->addChild(childLevel2);
   
   Matrix matrix;
   matrix.setTranslation( Vector( 5, 0, 0 ) );

   root.setLocalMtx(matrix);
   CPPUNIT_ASSERT_EQUAL(matrix, childLevel2->getGlobalMtx());
};

///////////////////////////////////////////////////////////////////////////////

TEST(UpdatingGlobalMatrices, sequenceOfUpdatesInfluencingChild)
{
   Node root("root");
   Node* child = new Node("child");
   root.addChild(child);

   // 1st update
   Matrix rootLocalMatrix; rootLocalMatrix.setTranslation( Vector( 5, 0, 0 ) );
   root.setLocalMtx(rootLocalMatrix);
   CPPUNIT_ASSERT_EQUAL(rootLocalMatrix, child->getGlobalMtx());

   // 2nd update
   rootLocalMatrix.setTranslation( Vector( 10, 0, 0 ) );
   root.setLocalMtx(rootLocalMatrix);
   CPPUNIT_ASSERT_EQUAL(rootLocalMatrix, child->getGlobalMtx());
};

///////////////////////////////////////////////////////////////////////////////
