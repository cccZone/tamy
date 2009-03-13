#include "TestFramework.h"
#include "MeshDefinition.h"
#include "TreeAnimator.h"
#include "Skeleton.h"
#include "AnimationDefinitionWriter.h"
#include <d3dx9.h>


//////////////////////////////////////////////////////////////////////////////

TEST(TreeAnimator, trunkAnimation)
{
   D3DXVECTOR3 zAxis(0, 0, 1);
   MeshDefinition treeMesh;
   treeMesh.isSkin = true;
   treeMesh.name = "tree";
   D3DXMatrixRotationAxis(&treeMesh.localMtx, &zAxis, D3DXToRadian(45));

   BonesInfluenceDefinition bonesForAttrib; 
   bonesForAttrib.push_back("treeBone_0");
   bonesForAttrib.push_back("treeBone_1");
   treeMesh.bonesInfluencingAttribute.push_back(bonesForAttrib);

   
   MeshDefinition* treeBone_0_Mesh = new MeshDefinition();
   treeBone_0_Mesh->name = "treeBone_0";
   treeBone_0_Mesh->isSkin = false;
   D3DXMatrixIdentity(&(treeBone_0_Mesh->localMtx));
   treeMesh.children.push_back(treeBone_0_Mesh);

   MeshDefinition* treeBone_1_Mesh = new MeshDefinition();
   treeBone_1_Mesh->name = "treeBone_1";
   treeBone_1_Mesh->isSkin = false;
   D3DXMATRIX mtxCombined = treeBone_0_Mesh->localMtx;
   D3DXMatrixTranslation(&(treeBone_1_Mesh->localMtx), 0, 10, 0);
   treeBone_0_Mesh->children.push_back(treeBone_1_Mesh);

   // create the animation for the skeleton
   TreeAnimator animator;
   AnimationDefinition animation;
   
   D3DXVECTOR3 windDirection(1, 0, 0);
   float windStrength = 10;
   float animationLengthInSecs = 8;
   animator(treeMesh, windDirection, windStrength, animationLengthInSecs, animation);

   // prepare the expected animation
   AnimationDefinition expectedAnimation;
   expectedAnimation.animSets.push_back(AnimationSetDefinition());
   AnimationSetDefinition& windAnimation = expectedAnimation.animSets.back();
   windAnimation.name = "wind";

   windAnimation.boneAnims.push_back(BoneAnimDefinition(2, 5, 2));
   BoneAnimDefinition& treeBone_0_Anim = windAnimation.boneAnims.back();
   windAnimation.boneAnims.push_back(BoneAnimDefinition(2, 5, 2));
   BoneAnimDefinition& treeBone_1_Anim = windAnimation.boneAnims.back();

   treeBone_0_Anim.boneName = treeBone_0_Mesh->name;
   treeBone_1_Anim.boneName = treeBone_1_Mesh->name;

   // first let's set up the keys that need to be there but don't change - the scale and translation keys
   treeBone_0_Anim.scaleKeysArr[0].Time = 0;
   treeBone_0_Anim.scaleKeysArr[0].Value = D3DXVECTOR3(1, 1, 1);
   treeBone_0_Anim.scaleKeysArr[1].Time = (float)(windAnimation.ticksPerSec * animationLengthInSecs);
   treeBone_0_Anim.scaleKeysArr[1].Value = treeBone_0_Anim.scaleKeysArr[0].Value;

   treeBone_0_Anim.translationKeysArr[0].Time = 0;
   treeBone_0_Anim.translationKeysArr[0].Value.x = treeBone_0_Mesh->localMtx._41;
   treeBone_0_Anim.translationKeysArr[0].Value.y = treeBone_0_Mesh->localMtx._42;
   treeBone_0_Anim.translationKeysArr[0].Value.z = treeBone_0_Mesh->localMtx._43;
   treeBone_0_Anim.translationKeysArr[1].Time = (float)(windAnimation.ticksPerSec * animationLengthInSecs);
   treeBone_0_Anim.translationKeysArr[1].Value = treeBone_0_Anim.translationKeysArr[0].Value;

   treeBone_1_Anim.scaleKeysArr[0].Time = 0;
   treeBone_1_Anim.scaleKeysArr[0].Value = D3DXVECTOR3(1, 1, 1);
   treeBone_1_Anim.scaleKeysArr[1].Time = (float)(windAnimation.ticksPerSec * animationLengthInSecs);
   treeBone_1_Anim.scaleKeysArr[1].Value = treeBone_1_Anim.scaleKeysArr[0].Value;

   treeBone_1_Anim.translationKeysArr[0].Time = 0;
   treeBone_1_Anim.translationKeysArr[0].Value.x = treeBone_1_Mesh->localMtx._41;
   treeBone_1_Anim.translationKeysArr[0].Value.y = treeBone_1_Mesh->localMtx._42;
   treeBone_1_Anim.translationKeysArr[0].Value.z = treeBone_1_Mesh->localMtx._43;
   treeBone_1_Anim.translationKeysArr[1].Time = (float)(windAnimation.ticksPerSec * animationLengthInSecs);
   treeBone_1_Anim.translationKeysArr[1].Value = treeBone_1_Anim.translationKeysArr[0].Value;

   // now for the fun part - the rotation keys...
   // ... they don't change in case of the branch's root
   for (char i = 0; i < 5; ++i)
   {
      treeBone_0_Anim.rotationKeysArr[i].Time = (float)(windAnimation.ticksPerSec) * i * 2.f;
      treeBone_0_Anim.rotationKeysArr[i].Value = D3DXQUATERNION(-0, -0, -0, 1);
   }

   // ... but they sway back and forth as we go higher
   treeBone_1_Anim.rotationKeysArr[0].Time = 0;
   treeBone_1_Anim.rotationKeysArr[0].Value = D3DXQUATERNION(-0, -0, -0.585097f, 0.810963f); 
   treeBone_1_Anim.rotationKeysArr[1].Time = 9600;
   treeBone_1_Anim.rotationKeysArr[1].Value = D3DXQUATERNION(-0, -0, 0.585097f, 0.810963f); 
   treeBone_1_Anim.rotationKeysArr[2].Time = 19200;
   treeBone_1_Anim.rotationKeysArr[2].Value = D3DXQUATERNION(-0, -0, 0.954086f, -0.299534f); 
   treeBone_1_Anim.rotationKeysArr[3].Time = 28800;
   treeBone_1_Anim.rotationKeysArr[3].Value = D3DXQUATERNION(-0, -0, 0.585097f, 0.810963f); 
   treeBone_1_Anim.rotationKeysArr[4].Time = 38400;
   treeBone_1_Anim.rotationKeysArr[4].Value = D3DXQUATERNION(-0, -0, -0.585097f, 0.810963f); 


   // compare the results
   AnimationDefinitionWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(expectedAnimation), writer(animation));
}

//////////////////////////////////////////////////////////////////////////////
