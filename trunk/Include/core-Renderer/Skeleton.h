#pragma once

#include <list>
#include <string>
#include <d3dx9.h>
#include <set>
#include <vector>
#include <map>


///////////////////////////////////////////////////////////////////////////////

class Node;
struct AnimationDefinition;
struct AnimationSetDefinition;
struct BoneAnimDefinition;

///////////////////////////////////////////////////////////////////////////////

/**
 * A skeleton represents a hierarchy of nodes along with
 * a set of animations that operate on it
 */
class Skeleton
{
private:
   Node* m_skeletonRootBone;
   ID3DXAnimationController* m_animationController;
   std::map<std::string, int> m_animPerTrack;
   std::vector<ID3DXAnimationSet*> m_animSets;

public:
   /**
    * A skeleton is created based on a template definition of
    * a set of animations and a skeletal structure of bones.
    *
    * The idea is that the bones in the skeletal structure of bones
    * (described by the 'skeletonRootBone' param) need to bear names
    * matching those of the bone animation defintions in the animation
    * definition template. There may be more bones there, but all the 
    * slots need to be filled.
    *
    * @throws std::invalid_argument - flags the lack of bones for some animations,
    *                                 or if the skeleton contains two or more
    *                                 nodes with the same name
    */
   Skeleton(const AnimationDefinition& animTemplate, Node& skeletonRootBone);
   ~Skeleton();

   /**
    * Activates an animation sequence
    *
    * @throws std::invalid_argument if the animation with this name doesn't exist
    */
   void activateAnimation(const std::string& animationName, bool enable);

   /**
    * @see activateAnimation method counterpart
    */
   bool isActive(const std::string& animationName) const;

   /**
    * Returns the length of an animation (in seconds)
    *
    * @throws std::invalid_argument if the animation with this name doesn't exist
    */
   float getAnimationLength(const std::string& animationName) const;

   /**
    * Sets the position in the animation sequence.
    * Obeys the rules of wrapping etc. if the passed position is
    * larger than the periodic length of the track
    *
    * @param position - specifies the position (in seconds)
    * @throws std::invalid_argument if the animation with this name doesn't exist
    */
   void setPosition(const std::string& animationName, float position) const;

   /**
    * @see setPosition method counterpart
    */
   float getPosition(const std::string& animationName) const;

   /**
    * This method sets the weight of an animation so that it can be blended
    * with other animations
    *
    * @param weight - <0, 1> corrsponds to <0%, 100%> range. Greater (or lesser) values
    *                 are acceptable as well
    * @throws std::invalid_argument if the animation with this name doesn't exist
    */
   void setBlendWeight(const std::string& animationName, float weight) const;

   /**
    * @see setBlendWeight method counterpart
    */
   float getBlendWeight(const std::string& animationName) const;

   /**
    * This method sets the speed of an animation
    *
    * @param speed - 1 - 100%
    *                2 - 200%, 
    *                0.5 - 50%, 
    *                -1 - play backwards with normal speed 
    *                0 - even though the animation is active, don't play it
    *                etc.
    *
    * @throws std::invalid_argument if the animation with this name doesn't exist
    */
   void setSpeed(const std::string& animationName, float speed) const;

   /**
    * @see setSpeed method counterpart
    */
   float getSpeed(const std::string& animationName) const;

   /**
    * Updates the global time line, thus moving the active animations forward
    */
   void update(float timeElapsed);

private:
   void createAnimationController(DWORD requiredBonesCount, 
                                  const AnimationDefinition& animTemplate);
   DWORD registerBoneStructure(Node& skeletonRootBone, 
                               std::set<std::string>& bonesSet);

   inline int getTrackForAnimation(const std::string& animationName) const;
};

///////////////////////////////////////////////////////////////////////////////

/** 
 * A complete skeleton will be created according to the data
 * stored in this structure
 */
struct AnimationDefinition
{
   std::list<AnimationSetDefinition> animSets;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This structure holds data that represent a single
 * animation (like "walk" or "idle")
 */
struct AnimationSetDefinition
{
   std::string name;
   double ticksPerSec;
   D3DXPLAYBACK_TYPE playbackType;

   std::list<BoneAnimDefinition> boneAnims;

   AnimationSetDefinition()
      : ticksPerSec(4800), playbackType(D3DXPLAY_LOOP)
   {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This structure represents a series of keyframes that 
 * animate a specific bone of a skeleton in an animation
 */
struct BoneAnimDefinition
{
   std::string boneName;
   UINT scaleKeysCount;
   UINT rotationKeysCount;
   UINT translationKeysCount;
   D3DXKEY_VECTOR3* scaleKeysArr;
   D3DXKEY_QUATERNION* rotationKeysArr;
   D3DXKEY_VECTOR3* translationKeysArr;

   BoneAnimDefinition(UINT _scaleKeysCount,
                      UINT _rotationKeysCount,
                      UINT _translationKeysCount)
         : scaleKeysCount(_scaleKeysCount),
         rotationKeysCount(_rotationKeysCount),
         translationKeysCount(_translationKeysCount)
   {
      if (scaleKeysCount > 0)
      {
         scaleKeysArr = new D3DXKEY_VECTOR3[scaleKeysCount];
      }
      else
      {
         scaleKeysArr = NULL;
      }

      if (rotationKeysCount > 0)
      {
         rotationKeysArr = new D3DXKEY_QUATERNION[rotationKeysCount];
      }
      else
      {
         rotationKeysArr = NULL;
      }

      if (translationKeysCount > 0)
      {
         translationKeysArr = new D3DXKEY_VECTOR3[translationKeysCount];
      }
      else
      {
         translationKeysArr = NULL;
      }
   }

   BoneAnimDefinition(const BoneAnimDefinition& rhs)
         : boneName(rhs.boneName),
         scaleKeysCount(rhs.scaleKeysCount),
         rotationKeysCount(rhs.rotationKeysCount),
         translationKeysCount(rhs.translationKeysCount)
   {
      if (scaleKeysCount > 0)
      {
         scaleKeysArr = new D3DXKEY_VECTOR3[scaleKeysCount];
         memcpy(scaleKeysArr, rhs.scaleKeysArr, sizeof(D3DXKEY_VECTOR3) * scaleKeysCount);
      }
      else
      {
         scaleKeysArr = NULL;
      }

      if (rotationKeysCount > 0)
      {
         rotationKeysArr = new D3DXKEY_QUATERNION[rotationKeysCount];
         memcpy(rotationKeysArr, rhs.rotationKeysArr, sizeof(D3DXKEY_QUATERNION) * rotationKeysCount);
      }
      else
      {
         rotationKeysArr = NULL;
      }

      if (translationKeysCount > 0)
      {
         translationKeysArr = new D3DXKEY_VECTOR3[translationKeysCount];
         memcpy(translationKeysArr, rhs.translationKeysArr, sizeof(D3DXKEY_VECTOR3) * translationKeysCount);
      }
      else
      {
         translationKeysArr = NULL;
      }
   }

   ~BoneAnimDefinition()
   {
      delete [] scaleKeysArr;
      scaleKeysArr = NULL;

      delete [] rotationKeysArr;
      rotationKeysArr = NULL;

      delete [] translationKeysArr;
      translationKeysArr = NULL;

      scaleKeysCount = 0;
      rotationKeysCount = 0;
      translationKeysCount = 0;
   }
};

///////////////////////////////////////////////////////////////////////////////
