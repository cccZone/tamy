#include "TreeAnimator.h"
#include "Skeleton.h"
#include "MeshDefinition.h"
#include <deque>
#include <stdexcept>
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

void TreeAnimator::operator()(const MeshDefinition& mesh, 
                              const D3DXVECTOR3& windDirection,
                              float windStrength,
                              float animationLengthInSecs,
                              AnimationDefinition& outAnimation)
{
   std::list<BoneCandidate> bones;
   unsigned int skeletonHeight = 0;
   extractSkeletonInfo(mesh, bones, skeletonHeight);
   skeletonHeight++;

   if (bones.size() == 0)
   {
      throw std::invalid_argument("Invalid mesh definition - no bones present in it");
   }

   outAnimation.animSets.push_back(AnimationSetDefinition());
   AnimationSetDefinition& windAnim = outAnimation.animSets.back();
   windAnim.name = "wind";

   for (std::list<BoneCandidate>::const_iterator it = bones.begin(); 
        it != bones.end(); ++it)
   {
      const BoneCandidate& boneCand = *it;

      windAnim.boneAnims.push_back(BoneAnimDefinition(2, 5, 2));
      BoneAnimDefinition& boneAnim = windAnim.boneAnims.back();

      boneAnim.boneName = boneCand.name;

      boneAnim.scaleKeysArr[0].Time = 0;
      boneAnim.scaleKeysArr[0].Value = D3DXVECTOR3(1, 1, 1);
      boneAnim.scaleKeysArr[1].Time = (float)(windAnim.ticksPerSec * animationLengthInSecs);
      boneAnim.scaleKeysArr[1].Value = boneAnim.scaleKeysArr[0].Value;

      boneAnim.translationKeysArr[0].Time = 0;
      boneAnim.translationKeysArr[0].Value.x = boneCand.localMtx._41;
      boneAnim.translationKeysArr[0].Value.y = boneCand.localMtx._42;
      boneAnim.translationKeysArr[0].Value.z = boneCand.localMtx._43;
      boneAnim.translationKeysArr[1].Time = (float)(windAnim.ticksPerSec * animationLengthInSecs);
      boneAnim.translationKeysArr[1].Value = boneAnim.translationKeysArr[0].Value;

      animateBone(boneCand, skeletonHeight, animationLengthInSecs, 
                  (float)windAnim.ticksPerSec, 5, windStrength, windDirection, boneAnim);
   }
}

///////////////////////////////////////////////////////////////////////////////

void TreeAnimator::animateBone(const BoneCandidate& bone, 
                               unsigned int skeletonHeight, 
                               float animationLengthInSecs,
                               float ticksPerSec,
                               unsigned char keysCount, 
                               float windStrength,
                               const D3DXVECTOR3& windDirection,
                               BoneAnimDefinition& outAnim)
{
   static D3DXVECTOR3 globalYAxis(0, 1, 0);

   float delay = -((float)bone.height /(float)skeletonHeight);
   float movement = ((float)bone.height /(float)skeletonHeight);
   float timeFactor = ticksPerSec * (animationLengthInSecs / (float)(keysCount - 1));

   D3DXVECTOR3 boneRotAxis;
   D3DXVec3Cross(&boneRotAxis, &globalYAxis, &windDirection);

   for (unsigned char i = 0; i < keysCount; ++i)
   {
      outAnim.rotationKeysArr[i].Time = timeFactor * i;

      float input = (((float)i / (float)(keysCount - 1)) + delay) * (D3DX_PI * 2.0f);
      float cycle = (cosf(input) + 1.0f) / 2.0f;
      float angle = (cycle - 0.25f) * (movement * windStrength);
      
      D3DXQuaternionRotationMatrix(&(outAnim.rotationKeysArr[i].Value), &bone.localMtx);
      D3DXQUATERNION rotQuat;
      D3DXQuaternionRotationAxis(&rotQuat, &boneRotAxis, angle);
      D3DXQuaternionMultiply(&(outAnim.rotationKeysArr[i].Value), 
                             &(outAnim.rotationKeysArr[i].Value),
                             &rotQuat);
      D3DXQuaternionConjugate(&(outAnim.rotationKeysArr[i].Value), 
                              &(outAnim.rotationKeysArr[i].Value));

   }
}

///////////////////////////////////////////////////////////////////////////////

void TreeAnimator::extractSkeletonInfo(const MeshDefinition& mesh,
                                       std::list<BoneCandidate>& outBones,
                                       unsigned int& outSkeletonHeight) const
{
   outSkeletonHeight = 0;

   std::deque<std::pair<const MeshDefinition*, unsigned int> > meshesQueue;
   meshesQueue.push_back(std::make_pair(&mesh, 0));

   while (meshesQueue.size() > 0)
   {
      const MeshDefinition* currMesh = meshesQueue.front().first;
      unsigned int currHeight = meshesQueue.front().second;
      meshesQueue.pop_front();

      bool isBone = false;
      if ((currMesh->isSkin == false) && (currMesh->name.find("Bone") != std::string::npos))
      {
         outBones.push_back(BoneCandidate(currMesh->name, currMesh->localMtx, currHeight));
         if (currHeight > outSkeletonHeight) {outSkeletonHeight = currHeight;}
         isBone = true;
      }

      for (std::list<MeshDefinition*>::const_iterator it = currMesh->children.begin();
           it != currMesh->children.end(); ++it)
      {
         meshesQueue.push_back(std::make_pair(*it, isBone ? (currHeight + 1) : currHeight));
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
