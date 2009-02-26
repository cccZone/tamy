#include "TestFramework.h"
#include "Skeleton.h"
#include "MatrixWriter.h"
#include "Node.h"
#include "GraphicalEntityMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Skeleton, basicAnimation)
{
   D3DXKEY_VECTOR3 animStart = {0, D3DXVECTOR3(0, 0, 0)};
   D3DXKEY_VECTOR3 animEnd = {14400, D3DXVECTOR3(15, 30, 45)};

   // prepare the animation
   BoneAnimDefinition kneeAnim(0, 0, 2);
   kneeAnim.boneName = "knee";
   kneeAnim.translationKeysArr[0] = animStart;
   kneeAnim.translationKeysArr[1] = animEnd;

   AnimationSetDefinition walk;
   walk.name = "walk";
   walk.boneAnims.push_back(kneeAnim);

   AnimationDefinition anim;
   anim.animSets.push_back(walk);

   // prepare the nodes
   Node kneeNode("knee");

   Skeleton skeleton(anim, kneeNode);
   skeleton.activateAnimation("walk");

   // run the tests
   D3DXMATRIX expectedResult;

   // animation start
   D3DXMatrixTranslation(&expectedResult, 0, 0, 0);
   skeleton.update(0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, kneeNode.getLocalMtx());

   // 1/3rd through
   D3DXMatrixTranslation(&expectedResult, 5, 10, 15);
   skeleton.update(1);
   CPPUNIT_ASSERT_EQUAL(expectedResult, kneeNode.getLocalMtx());

   // 2/3rd through
   D3DXMatrixTranslation(&expectedResult, 10, 20, 30);
   skeleton.update(1);
   CPPUNIT_ASSERT_EQUAL(expectedResult, kneeNode.getLocalMtx());

   // animation end - it wraps around (by default it's in the looping mode...)
   D3DXMatrixTranslation(&expectedResult, 0, 0, 0);
   skeleton.update(1);
   CPPUNIT_ASSERT_EQUAL(expectedResult, kneeNode.getLocalMtx());
}

///////////////////////////////////////////////////////////////////////////////
