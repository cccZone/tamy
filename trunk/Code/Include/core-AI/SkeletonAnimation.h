/// @file   core-AI/SkeletonAnimation.h
/// @brief  scale-rotation-translation animation resource

#include "core/Resource.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class BoneSRTAnimation;
class BoneSRTAnimationPlayer;
class Node;
struct Vector;
struct Quaternion;

///////////////////////////////////////////////////////////////////////////////

class SkeletonAnimation : public Resource
{
   DECLARE_ALLOCATOR( SkeletonAnimation, AM_DEFAULT );
   DECLARE_RESOURCE();

private:
   float                                  m_animationLength;
   std::vector< BoneSRTAnimation* >       m_boneAnimations;

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   SkeletonAnimation( const FilePath& resourceName = FilePath() );
   ~SkeletonAnimation();

   /**
    * Adds a batch of animation keys.
    *
    * @param animationKeys
    */
   void addKeys( BoneSRTAnimation* animationKeys );

   /**
    * Adds an orientation key definition for the specified bone at the specified time.
    *
    * @param boneName
    * @param frameTime
    * @param orientation
    * @param translation
    */
   void addOrientationKey( const std::string& boneName, float frameTime, const Quaternion& orientation );

   /**
    * Adds a translation key definition for the specified bone at the specified time.
    *
    * @param boneName
    * @param frameTime
    * @param orientation
    * @param translation
    */
   void addTranslationKey( const std::string& boneName, float frameTime, const Vector& translation );

   /**
    * Resets the animation contents.
    */
   void clear();

   /**
    * Returns the animation length.
    */
   float getAnimationLength() const { return m_animationLength; }

   /**
    * Initializes an animation player.
    *
    * @param skeleton         skeleton on which the animation will be played
    * @Param outBonePlayers   a list of bone players matching the sequence of bones in the specified skeleton
    */
   void initializePlayer( const std::vector< Node* >& skeleton, std::vector< BoneSRTAnimationPlayer* >& outBonePlayers ) const;

   /**
    * Finds an animation stream for the specified bone.
    *
    * @param boneName
    * @return pointer to an animation stream
    */
   BoneSRTAnimation* getBoneDef( const std::string& boneName ) const;

   /**
    * Collects the names of the bones used in this animation.
    *
    * @param outNames
    */
   void collectBonesNames( std::vector< std::string>& outNames ) const;
};

///////////////////////////////////////////////////////////////////////////////
