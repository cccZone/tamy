#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Skeleton.h"
#include "core\MatrixWriter.h"
#include "core\Node.h"
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
   Node kneeNode("knee", false);

   Skeleton skeleton(anim, kneeNode);
   skeleton.activateAnimation("walk", true);

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

TEST(Skeleton, animationSequenceLengts)
{
   D3DXKEY_VECTOR3 animStart = {0, D3DXVECTOR3(0, 0, 0)};
   D3DXKEY_VECTOR3 animEnd = {14400, D3DXVECTOR3(15, 30, 45)};

   BoneAnimDefinition kneeAnim(0, 0, 2);
   kneeAnim.boneName = "knee";
   kneeAnim.translationKeysArr[0] = animStart;
   kneeAnim.translationKeysArr[1] = animEnd;

   AnimationSetDefinition walk;
   walk.name = "walk";
   walk.boneAnims.push_back(kneeAnim);

   AnimationDefinition anim;
   anim.animSets.push_back(walk);

   Node kneeNode("knee", false);

   Skeleton skeleton(anim, kneeNode);
   

   CPPUNIT_ASSERT_EQUAL(3.f, skeleton.getAnimationLength("walk"));
}

///////////////////////////////////////////////////////////////////////////////

TEST(Skeleton, settingAnimationPosition)
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
   Node kneeNode("knee", false);

   Skeleton skeleton(anim, kneeNode);
   skeleton.activateAnimation("walk", true);


   // run the tests
   skeleton.setPosition("walk", 1);
   D3DXMATRIX expectedResult;

   // we're already as if 1/3rd through
   D3DXMatrixTranslation(&expectedResult, 5, 10, 15);
   skeleton.update(0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, kneeNode.getLocalMtx());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Skeleton, blendingTwoAnimations)
{
   D3DXKEY_VECTOR3 walkAnimStart = {0, D3DXVECTOR3(0, 0, 0)};
   D3DXKEY_VECTOR3 walkAnimEnd = {19200, D3DXVECTOR3(4, 4, 4)};

   D3DXKEY_VECTOR3 runAnimStart = {0, D3DXVECTOR3(0, 0, 0)};
   D3DXKEY_VECTOR3 runAnimEnd = {19200, D3DXVECTOR3(12, 12, 12)};

   // prepare the animations
   BoneAnimDefinition walkKneeAnim(0, 0, 2);
   walkKneeAnim.boneName = "knee";
   walkKneeAnim.translationKeysArr[0] = walkAnimStart;
   walkKneeAnim.translationKeysArr[1] = walkAnimEnd;
   AnimationSetDefinition walk;
   walk.name = "walk";
   walk.boneAnims.push_back(walkKneeAnim);

   BoneAnimDefinition runKneeAnim(0, 0, 2);
   runKneeAnim.boneName = "knee";
   runKneeAnim.translationKeysArr[0] = runAnimStart;
   runKneeAnim.translationKeysArr[1] = runAnimEnd;
   AnimationSetDefinition run;
   run.name = "run";
   run.boneAnims.push_back(runKneeAnim);

   AnimationDefinition anim;
   anim.animSets.push_back(walk);
   anim.animSets.push_back(run);

   // prepare the nodes
   Node kneeNode("knee", false);

   Skeleton skeleton(anim, kneeNode);
   skeleton.activateAnimation("walk", true);
   skeleton.activateAnimation("run", true);


   // run the tests
   D3DXMATRIX expectedResult;

   skeleton.setBlendWeight("walk", 1);
   skeleton.setBlendWeight("run", 0);
   skeleton.update(1);
   D3DXMatrixTranslation(&expectedResult, 1, 1, 1);
   CPPUNIT_ASSERT_EQUAL(expectedResult, kneeNode.getLocalMtx());

   skeleton.setBlendWeight("walk", 0.5f);
   skeleton.setBlendWeight("run", 0.5f);
   skeleton.update(1);
   D3DXMatrixTranslation(&expectedResult, 4, 4, 4);
   CPPUNIT_ASSERT_EQUAL(expectedResult, kneeNode.getLocalMtx());

   skeleton.setBlendWeight("walk", 0);
   skeleton.setBlendWeight("run", 1);
   skeleton.update(1);
   D3DXMatrixTranslation(&expectedResult, 9, 9, 9);
   CPPUNIT_ASSERT_EQUAL(expectedResult, kneeNode.getLocalMtx());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Skeleton, changingAnimationSpeed)
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
   Node kneeNode("knee", false);

   Skeleton skeleton(anim, kneeNode);
   skeleton.activateAnimation("walk", true);


   // run the tests
   D3DXMATRIX expectedResult;

   skeleton.setSpeed("walk", 0.5f);
   skeleton.update(1);
   D3DXMatrixTranslation(&expectedResult, 2.5f, 5, 7.5f);
   CPPUNIT_ASSERT_EQUAL(expectedResult, kneeNode.getLocalMtx());

   skeleton.setSpeed("walk", 2);
   skeleton.update(1);
   D3DXMatrixTranslation(&expectedResult, 12.5f, 25, 37.5f);
   CPPUNIT_ASSERT_EQUAL(expectedResult, kneeNode.getLocalMtx());
}

///////////////////////////////////////////////////////////////////////////////
