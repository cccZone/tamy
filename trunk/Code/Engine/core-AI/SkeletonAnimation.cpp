#include "core-AI/SkeletonAnimation.h"
#include "core-AI/BoneSRTAnimation.h"
#include "core/Node.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( SkeletonAnimation, tan, AM_BINARY );
   PROPERTY( float, m_animationLength );
   PROPERTY( std::vector< BoneSRTAnimation* >, m_boneAnimations );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

SkeletonAnimation::SkeletonAnimation( const FilePath& resourceName )
   : Resource( resourceName )
   , m_animationLength( 0.f )
{

}

///////////////////////////////////////////////////////////////////////////////

SkeletonAnimation::~SkeletonAnimation()
{
   clear();
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimation::addKeys( BoneSRTAnimation* animationKeys )
{
   // update total animation length
   float boneAnimLength = animationKeys->getDuration();
   if ( boneAnimLength > m_animationLength )
   {
      m_animationLength = boneAnimLength;
   }

   // find the entry corresponding to the bone
   unsigned int bonesCount = m_boneAnimations.size();
   for ( unsigned int boneIdx = 0; boneIdx < bonesCount; ++boneIdx )
   {
      BoneSRTAnimation* boneDef = m_boneAnimations[boneIdx];
      if ( boneDef->getBoneName() == animationKeys->getBoneName() )
      {
         // an animation definition for this bone already exists - replace it
         delete boneDef;
         m_boneAnimations[boneIdx] = animationKeys;
         return;
      }
   }

   // if we got this far, it means that an animation definition for the specified
   // bone doesn't exist - create one
   m_boneAnimations.push_back( animationKeys );
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimation::addOrientationKey( const std::string& boneName, float frameTime, const D3DXQUATERNION& orientation )
{
   BoneSRTAnimation* animDef = getBoneDef( boneName );
   if ( animDef == NULL )
   {
      animDef = new BoneSRTAnimation( boneName );
      m_boneAnimations.push_back( animDef );
   }

   animDef->addOrientationKey( frameTime, orientation );

   // update total animation length
   float boneAnimLength = animDef->getDuration();
   if ( boneAnimLength > m_animationLength )
   {
      m_animationLength = boneAnimLength;
   }
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimation::addTranslationKey( const std::string& boneName, float frameTime, const D3DXVECTOR3& translation )
{
   BoneSRTAnimation* animDef = getBoneDef( boneName );
   if ( animDef == NULL )
   {
      animDef = new BoneSRTAnimation( boneName );
      m_boneAnimations.push_back( animDef );
   }

   animDef->addTranslationKey( frameTime, translation );

   // update total animation length
   float boneAnimLength = animDef->getDuration();
   if ( boneAnimLength > m_animationLength )
   {
      m_animationLength = boneAnimLength;
   }
}

///////////////////////////////////////////////////////////////////////////////

BoneSRTAnimation* SkeletonAnimation::getBoneDef( const std::string& boneName ) const
{
   // find the entry corresponding to the bone
   unsigned int bonesCount = m_boneAnimations.size();
   for ( unsigned int boneIdx = 0; boneIdx < bonesCount; ++boneIdx )
   {
      BoneSRTAnimation* boneDef = m_boneAnimations[boneIdx];
      if ( boneDef->getBoneName() == boneName )
      {
         return boneDef;
      }
   }

   return NULL;
}


///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimation::clear()
{
   m_animationLength = 0.0f;
   
   unsigned int bonesCount = m_boneAnimations.size();
   for ( unsigned int i = 0; i < bonesCount; ++i )
   {
      delete m_boneAnimations[i];
   }
   m_boneAnimations.clear();
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimation::initializePlayer( const std::vector< Node* >& skeleton, std::vector< BoneSRTAnimationPlayer* >& outBonePlayers ) const
{
   // make sure the specified output list is empty - otherwise - erase it
   unsigned int count = outBonePlayers.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete outBonePlayers[i];
   }
   outBonePlayers.clear();

   // go through the skeleton and create players for particular bones
   count = skeleton.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      BoneSRTAnimation* anim = getBoneDef( skeleton[i]->getName() );
      if ( anim == NULL )
      {
         outBonePlayers.push_back( NULL );
      }
      else
      {
         outBonePlayers.push_back( new BoneSRTAnimationPlayer( *anim ) );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimation::collectBonesNames( std::vector< std::string>& outNames ) const
{
   unsigned int count = m_boneAnimations.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      outNames.push_back( m_boneAnimations[i]->getBoneName() );
   }
}

///////////////////////////////////////////////////////////////////////////////
