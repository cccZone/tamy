#include "core-AI/BoneSRTAnimation.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( BoneSRTAnimation, Object )
   PROPERTY( std::string, m_boneName )
   PROPERTY( std::vector< float >, m_orientation.m_time )
   PROPERTY( std::vector< D3DXQUATERNION >, m_orientation.m_keys )
   PROPERTY( std::vector< float >, m_translation.m_time )
   PROPERTY( std::vector< D3DXVECTOR3 >, m_translation.m_keys )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

BoneSRTAnimation::BoneSRTAnimation( const std::string& boneName )
   : m_boneName( boneName )
   , m_duration( 0.0f )
{

}

///////////////////////////////////////////////////////////////////////////////

void BoneSRTAnimation::onObjectLoaded()
{
   __super::onObjectLoaded();

   updateDuration();
}


///////////////////////////////////////////////////////////////////////////////

void BoneSRTAnimation::addOrientationKey( float time, const D3DXQUATERNION& orientation )
{
   m_orientation.addKey( time, orientation );

   // update the animation duration
   updateDuration();
}

///////////////////////////////////////////////////////////////////////////////

void BoneSRTAnimation::addTranslationKey( float time, const D3DXVECTOR3& translation )
{
   m_translation.addKey( time, translation );

   // update the animation duration
   updateDuration();
}

///////////////////////////////////////////////////////////////////////////////

void BoneSRTAnimation::updateDuration()
{
   m_duration = 0.0f;

   if ( m_orientation.m_time.empty() == false && m_duration < m_orientation.m_time.back() )
   {
      m_duration = m_orientation.m_time.back();
   }

   if ( m_translation.m_time.empty() == false && m_duration < m_translation.m_time.back() )
   {
      m_duration = m_translation.m_time.back();
   }
}

///////////////////////////////////////////////////////////////////////////////

bool BoneSRTAnimation::getTranslation( unsigned int& lastCheckedKeyIdx, float time, D3DXVECTOR3& outTranslation ) const
{
   return m_translation.getKey( lastCheckedKeyIdx, time, outTranslation );
}

///////////////////////////////////////////////////////////////////////////////

bool BoneSRTAnimation::getOrientation( unsigned int& lastCheckedKeyIdx, float time, D3DXQUATERNION& outOrientation ) const
{
   return m_orientation.getKey( lastCheckedKeyIdx, time, outOrientation );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BoneSRTAnimationPlayer::BoneSRTAnimationPlayer( const BoneSRTAnimation& animation )
   : m_animation( animation )
   , m_orientationKeyIdx( 0 )
   , m_translationKeyIdx( 0 )
{

}

///////////////////////////////////////////////////////////////////////////////

bool BoneSRTAnimationPlayer::getTranslation( float time, D3DXVECTOR3& outTranslation )
{
   return m_animation.getTranslation( m_translationKeyIdx, time, outTranslation );
}

///////////////////////////////////////////////////////////////////////////////

bool BoneSRTAnimationPlayer::getOrientation( float time, D3DXQUATERNION& outOrientation )
{
   return m_animation.getOrientation( m_orientationKeyIdx, time, outOrientation );
}

///////////////////////////////////////////////////////////////////////////////
