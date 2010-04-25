#include "core-Renderer\Skeleton.h"
#include "core\Node.h"
#include <set>
#include <deque>
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

Skeleton::Skeleton(const AnimationDefinition& animTemplate, Node& skeletonRootBone)
      : m_skeletonRootBone(&skeletonRootBone),
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
      DWORD registeredBonesCount = registerBoneStructure(skeletonRootBone, bonesSet);
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
   m_animPerTrack.clear();
   for (unsigned int i = 0; i < m_animSets.size(); ++i)
   {
      m_animSets.at(i)->Release();
   }
   m_animSets.clear();

   m_skeletonRootBone = NULL;

   m_animationController->Release();
   m_animationController = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::createAnimationController(DWORD requiredBonesCount, const AnimationDefinition& animTemplate)
{
   DWORD animationSetsCount = animTemplate.animSets.size();

   // we're gonna set the same number of tracks as the number of available animations,
   // so that we can activate them all at once
   DWORD animationTracksCount = animTemplate.animSets.size();

   // create an animation controller
   HRESULT res;
   if ((requiredBonesCount == 0) || (animTemplate.animSets.size() == 0))
   {
      res = D3DXCreateAnimationController(1, 1, 1, 1, &m_animationController);
   }
   else
   {
      res = D3DXCreateAnimationController(requiredBonesCount, 
                                          animationSetsCount, 
                                          animationTracksCount,
                                          1, // just one event (otherwise the method fails) - we don't need any for now though...
                                          &m_animationController);
   }
   if (FAILED(res) || (m_animationController == NULL))
   {
      throw std::runtime_error("Can't create an animation controller");
   }

   // create the animations and register them with the controller
   int animSetId = 0;
   for (std::list<AnimationSetDefinition>::const_iterator animSetIt = animTemplate.animSets.begin();
        animSetIt != animTemplate.animSets.end(); ++animSetIt, ++animSetId)
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

      m_animPerTrack.insert(std::make_pair(animSetDef.name, animSetId));
      m_animSets.push_back(newAnimSet);
      newAnimSet->AddRef();

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

   // set up all the animatin tracks descriptions to the default values
   for (DWORD i = 0; i < animationTracksCount; ++i)
   {
      m_animationController->SetTrackEnable(i, false);
      m_animationController->SetTrackSpeed(i, 1);
      m_animationController->SetTrackWeight(i, 1);
      m_animationController->SetTrackPriority(i, D3DXPRIORITY_HIGH);
      m_animationController->SetTrackPosition(i, 0);
   }

   // at this point the animation controller should exist !
   assert(m_animationController != NULL);
}

///////////////////////////////////////////////////////////////////////////////

DWORD Skeleton::registerBoneStructure(Node& skeletonRootBone, std::set<std::string>& bonesSet)
{
   // animation controller needs to exist in order to perform this task
   assert(m_animationController != NULL);

   // we'll use BFS to explore the nodes structure and register the matrices with the animation controller
   std::set<std::string> distinctBoneNamesUsed;
   std::deque<Node*> nodesQueue;
   nodesQueue.push_back(&skeletonRootBone);
   
   HRESULT res;

   while ((nodesQueue.size() > 0) && (bonesSet.size() > 0))
   {
      Node* currentNode = nodesQueue.front();
      nodesQueue.pop_front();

      // add all the children of the node for the BFS to search through
      const std::list<Node*>& children = currentNode->getChildren();
      for (std::list<Node*>::const_iterator childNodeIt = children.begin();
           childNodeIt != children.end(); ++childNodeIt)
      {
         nodesQueue.push_back(*childNodeIt);
      }

      // check if the node is the one of the ones we're looking for
      const std::string& nodeName = currentNode->getName();
      std::set<std::string>::iterator requiredBoneIt = bonesSet.find(nodeName);
      if (requiredBoneIt == bonesSet.end()) 
      {
         continue;
      }
      else
      {
         bonesSet.erase(requiredBoneIt);
      }

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
   }

   return distinctBoneNamesUsed.size();
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::activateAnimation(const std::string& animationName, bool enable)
{
   int trackId = getTrackForAnimation(animationName);
   ID3DXAnimationSet* animSet = m_animSets.at(trackId);

   HRESULT res = m_animationController->SetTrackAnimationSet(trackId, animSet);
   if (FAILED(res))
   {
      throw std::invalid_argument(std::string("Animation '") + animationName + 
                                  std::string("' could not be activated"));
   }

   m_animationController->SetTrackEnable(trackId, enable);
}

///////////////////////////////////////////////////////////////////////////////

bool Skeleton::isActive(const std::string& animationName) const
{
   int trackId = getTrackForAnimation(animationName);

   D3DXTRACK_DESC desc;
   m_animationController->GetTrackDesc(trackId, &desc);

   return (bool)desc.Enable;
}

///////////////////////////////////////////////////////////////////////////////

float Skeleton::getAnimationLength(const std::string& animationName) const
{
   int trackId = getTrackForAnimation(animationName);
   ID3DXAnimationSet* animSet = m_animSets.at(trackId);
   float length = static_cast<float> (animSet->GetPeriod());
   return length;
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::setPosition(const std::string& animationName, 
                           float position) const
{
   int trackId = getTrackForAnimation(animationName);
   m_animationController->SetTrackPosition(trackId, position);
}

///////////////////////////////////////////////////////////////////////////////

float Skeleton::getPosition(const std::string& animationName) const
{
   int trackId = getTrackForAnimation(animationName);

   D3DXTRACK_DESC desc;
   m_animationController->GetTrackDesc(trackId, &desc);

   return static_cast<float> (desc.Position);
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::setBlendWeight(const std::string& animationName, 
                              float weight) const
{
   int trackId = getTrackForAnimation(animationName);
   m_animationController->SetTrackWeight(trackId, weight);
}

///////////////////////////////////////////////////////////////////////////////

float Skeleton::getBlendWeight(const std::string& animationName) const
{
   int trackId = getTrackForAnimation(animationName);

   D3DXTRACK_DESC desc;
   m_animationController->GetTrackDesc(trackId, &desc);

   return static_cast<float> (desc.Weight);
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::setSpeed(const std::string& animationName, float speed) const
{
   int trackId = getTrackForAnimation(animationName);
   m_animationController->SetTrackSpeed(trackId, speed);
}

///////////////////////////////////////////////////////////////////////////////

float Skeleton::getSpeed(const std::string& animationName) const
{
   int trackId = getTrackForAnimation(animationName);

   D3DXTRACK_DESC desc;
   m_animationController->GetTrackDesc(trackId, &desc);

   return static_cast<float> (desc.Speed);
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::update(float timeElapsed)
{
   m_animationController->AdvanceTime(timeElapsed, NULL);
}

///////////////////////////////////////////////////////////////////////////////

int Skeleton::getTrackForAnimation(const std::string& animationName) const
{
   AnimationsMap::const_iterator it = m_animPerTrack.find(animationName);
   if (it == m_animPerTrack.end())
   {
      throw std::invalid_argument(std::string("Animation '") + animationName + 
                                  std::string("' doesn't exist"));
   }

   return it->second;
}

///////////////////////////////////////////////////////////////////////////////

void Skeleton::getAnimationNames(std::set<std::string>& names) const
{
   for (AnimationsMap::const_iterator it = m_animPerTrack.begin();
        it != m_animPerTrack.end(); ++it)
   {
      names.insert(it->first);
   }
}

///////////////////////////////////////////////////////////////////////////////
