#include "core-AI/BoneSRTAnimation.h"
#include "core/Assert.h"
#include "core/Quaternion.h"
#include "core/Vector.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( BoneSRTAnimation );
   PARENT( ReflectionObject);
   PROPERTY( std::string, m_boneName );
   PROPERTY( Array< float >, m_orientation.m_time );
   PROPERTY( Array< Quaternion >, m_orientation.m_keys );
   PROPERTY( Array< float >, m_translation.m_time );
   PROPERTY( Array< Vector >, m_translation.m_keys );
END_OBJECT();

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

void BoneSRTAnimation::addOrientationKey( float time, const Quaternion& orientation )
{
   m_orientation.addKey( time, orientation );

   // update the animation duration
   updateDuration();
}

///////////////////////////////////////////////////////////////////////////////

void BoneSRTAnimation::addTranslationKey( float time, const Vector& translation )
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

bool BoneSRTAnimation::getTranslation( unsigned int& lastCheckedKeyIdx, float time, Vector& outTranslation ) const
{
   return m_translation.getKey( lastCheckedKeyIdx, time, outTranslation );
}

///////////////////////////////////////////////////////////////////////////////

bool BoneSRTAnimation::getOrientation( unsigned int& lastCheckedKeyIdx, float time, Quaternion& outOrientation ) const
{
   return m_orientation.getKey( lastCheckedKeyIdx, time, outOrientation );
}

///////////////////////////////////////////////////////////////////////////////

void BoneSRTAnimation::getTranslationKey( unsigned int keyIdx, Vector& outValue, float& outTime ) const 
{ 
   outValue = m_translation.m_keys[keyIdx]; 
   outTime = m_translation.m_time[keyIdx]; 
}

///////////////////////////////////////////////////////////////////////////////

void BoneSRTAnimation::setTranslationKey( unsigned int keyIdx, const Vector& value )
{
   m_translation.m_keys[keyIdx] = value;
}

///////////////////////////////////////////////////////////////////////////////

void BoneSRTAnimation::getOrientationKey( unsigned int keyIdx, Quaternion& outValue, float& outTime ) const
{
   outValue = m_orientation.m_keys[keyIdx]; 
   outTime = m_orientation.m_time[keyIdx]; 
}

///////////////////////////////////////////////////////////////////////////////

void BoneSRTAnimation::setOrientationKey( unsigned int keyIdx, const Quaternion& value )
{
   m_orientation.m_keys[keyIdx] = value;
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

bool BoneSRTAnimationPlayer::getTranslation( float time, Vector& outTranslation )
{
   return m_animation.getTranslation( m_translationKeyIdx, time, outTranslation );
}

///////////////////////////////////////////////////////////////////////////////

bool BoneSRTAnimationPlayer::getOrientation( float time, Quaternion& outOrientation )
{
   return m_animation.getOrientation( m_orientationKeyIdx, time, outOrientation );
}

///////////////////////////////////////////////////////////////////////////////
