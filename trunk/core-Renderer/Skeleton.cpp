#include "Skeleton.h"
#include "Node.h"
#include <set>
#include <deque>
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

Skeleton::Skeleton(const AnimationDefinition& animTemplate, Node* skeletonRootBone)
      : m_skeletonRootBone(skeletonRootBone),
      m_animationController(NULL)
{
   // check how many bones are used (eliminate reappearing names)
   std::set<std::string> bonesSet;
   for (std::list<AnimationSetDefinition>::const_iterator animSetIt = animTemplate.animSets.begin();
        animSetIt != animTemplate.animSets.end(); ++animSetIt)
   {
      for (std::list<BoneAnimDefinition>::const_iterator boneIt = animSetIt->boneAnims.begin();
           boneIt != animSetIt->boneAnims.end(); ++boneIt)
      {
         bonesSet.insert(boneIt->boneName);
      }
   }

   try
   {
      DWORD requiredBonesCount = bonesSet.size();
      createAnimationController(requiredBonesCount, animTemplate);
      DWORD registeredBonesCount = registerBoneStructure(skeletonRootBone);
      if (registeredBonesCount < requiredBonesCount)
      {
         throw std::invalid_argument("There are bones missing in the passed bones hierarchy");
      }
   }
   catch(std::exception& ex)
   {
      // cleanup
      delete m_skeletonRootBone;
      m_skeletonRootBone = NULL;

      if (m_animationController != NULL)
      {
         m_animationController->Release();
         m_animationController = NULL;
      }

      throw ex;
   }

}

///////////////////////////////////////////////////////////////////////////////

Skeleton::~Skeleton()
{
   delete m_skeletonRootBone;
   m_skeletonRootBone = NULL;

   m_animationController->Release();
   m_animationController = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::createAnimationController(DWORD requiredBonesCount, const AnimationDefinition& animTemplate)
{
   // create an animation controller
   HRESULT res;
   if ((requiredBonesCount == 0) || (animTemplate.animSets.size() == 0))
   {
      res = D3DXCreateAnimationController(1, 1, 1, 1, &m_animationController);
   }
   else
   {
      // (1) we're gonna set the same number of tracks as the number of available animations,
      // so that we can activate them all at once
      res = D3DXCreateAnimationController(requiredBonesCount, 
                                          animTemplate.animSets.size(), 
                                          animTemplate.animSets.size(), // (1)
                                          1, // just one event (otherwise the method fails) - we don't need any for now though...
                                          &m_animationController);
   }
   if (FAILED(res) || (m_animationController == NULL))
   {
      throw std::runtime_error("Can't create an animation controller");
   }

   // create the animations and register them with the controller
   for (std::list<AnimationSetDefinition>::const_iterator animSetIt = animTemplate.animSets.begin();
        animSetIt != animTemplate.animSets.end(); ++animSetIt)
   {
      const AnimationSetDefinition& animSetDef = *animSetIt;
      ID3DXKeyframedAnimationSet* newAnimSet = NULL;
      res = D3DXCreateKeyframedAnimationSet(animSetDef.name.c_str(),
                                            animSetDef.ticksPerSec,
                                            animSetDef.playbackType,
                                            animSetDef.boneAnims.size(),
                                            0,
                                            NULL,
                                            &newAnimSet);

      if (FAILED(res) || (newAnimSet == NULL))
      {
         throw std::runtime_error("Can't create an animation set controller");
      }

      // register particular bone animation sequences
      for (std::list<BoneAnimDefinition>::const_iterator boneIt = animSetIt->boneAnims.begin();
           boneIt != animSetIt->boneAnims.end(); ++boneIt)
      {
         res = newAnimSet->RegisterAnimationSRTKeys(boneIt->boneName.c_str(),
                                                    boneIt->scaleKeysCount,
                                                    boneIt->rotationKeysCount,
                                                    boneIt->translationKeysCount,
                                                    boneIt->scaleKeysArr,
                                                    boneIt->rotationKeysArr,
                                                    boneIt->translationKeysArr,
                                                    NULL);
         if (FAILED(res))
         {
            newAnimSet->Release();
            throw std::runtime_error("Can't register an animation controller");
         }
      }

      // once the animation set is successfully created, register it with the animation controller
      m_animationController->RegisterAnimationSet(newAnimSet);
      newAnimSet->Release();
   }

   // at this point the animation controller should exist !
   assert(m_animationController != NULL);
}

///////////////////////////////////////////////////////////////////////////////

DWORD Skeleton::registerBoneStructure(Node* skeletonRootBone)
{
   // animation controller needs to exist in order to perform this task
   assert(m_animationController != NULL);

   if (skeletonRootBone == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a Node instance");
   }

   // we'll use BFS to explore the nodes structure and register the matrices with the animation controller
   std::set<std::string> distinctBoneNamesUsed;
   std::deque<Node*> nodesQueue;
   nodesQueue.push_back(skeletonRootBone);
   
   HRESULT res;

   while (nodesQueue.size() > 0)
   {
      Node* currentNode = nodesQueue.front();
      nodesQueue.pop_front();

      const std::string& nodeName = currentNode->getName();
      // check to see if the bone was previously used and if not, register its name
      // since we're using a set - we can accomplish both goals 
      // with one shot - using the 'insert' method
      if (distinctBoneNamesUsed.insert(nodeName).second == false)
      {
         throw std::invalid_argument(
            std::string("Same name assigned to more than two nodes in the skeleton: ") + 
            nodeName);
      }

      // register the matrix of the node as the animation output
      res = m_animationController->RegisterAnimationOutput(nodeName.c_str(),
                                                           &(currentNode->accessLocalMtx()),
                                                           NULL, 
                                                           NULL, 
                                                           NULL);
      if (FAILED(res))
      {
         throw std::runtime_error("Failed to register an animation output");
      }

      // add all teh childre of the node for the BFS to search through
      const std::list<Node*>& children = currentNode->getChildren();
      for (std::list<Node*>::const_iterator childNodeIt = children.begin();
           childNodeIt != children.end(); ++childNodeIt)
      {
         nodesQueue.push_back(*childNodeIt);
      }
   }

   return distinctBoneNamesUsed.size();
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::activateAnimation(const std::string& animationName)
{
   ID3DXAnimationSet* animSet = NULL;
   HRESULT res = m_animationController->GetAnimationSetByName(animationName.c_str(), &animSet);

   if (FAILED(res) || (animSet == NULL))
   {
      throw std::invalid_argument(std::string("Animation '") + animationName + 
                                  std::string("' doesn't exist"));
   }

   res = m_animationController->SetTrackAnimationSet(0, animSet);
   if (FAILED(res))
   {
      throw std::invalid_argument(std::string("Animation '") + animationName + 
                                  std::string("' could not be activated"));
   }
   m_animationController->SetTrackEnable(0, true);
   m_animationController->SetTrackSpeed(0, 1);
   m_animationController->SetTrackWeight(0, 1);
   m_animationController->SetTrackPriority(0, D3DXPRIORITY_HIGH);
   m_animationController->SetTrackPosition(0, 0);

   animSet->Release();
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::update(float timeElapsed)
{
   m_animationController->AdvanceTime(timeElapsed, NULL);

   ID3DXAnimationSet* animSet = NULL;
   m_animationController->GetTrackAnimationSet(0, &animSet);
   
   D3DXTRACK_DESC desc;
   m_animationController->GetTrackDesc(0, &desc);
   double trackPos = animSet->GetPeriodicPosition(desc.Position);
   D3DXVECTOR3 scale;
   D3DXQUATERNION rot;
   D3DXVECTOR3 trans;
   animSet->GetSRT(trackPos, 0, &scale, &rot, &trans);
   animSet->Release();
}

///////////////////////////////////////////////////////////////////////////////
