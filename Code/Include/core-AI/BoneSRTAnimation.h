/// @file   core-AI/BoneSRTAnimation.h
/// @brief  animation data stream for a single bone
#pragma once

#include "core/ReflectionObject.h"
#include "core-AI/AnimationTimeline.h"
#include "core/Vector.h"
#include "core/Quaternion.h"
#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class BoneSRTAnimationPlayer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Animation data stream for a single bone.
 */
class BoneSRTAnimation : public ReflectionObject
{
   DECLARE_ALLOCATOR( BoneSRTAnimation, AM_DEFAULT );
   DECLARE_CLASS();

private:
   struct VecLerp
   {
      DECLARE_ALLOCATOR( VecLerp, AM_DEFAULT );

   public:
      void operator()( Vector& outVec, const Vector& start, const Vector& end, const FastFloat& percentage ) const
      {
         outVec.setLerp( start, end, percentage );
      }
   };

   struct QuatLerp
   {
      DECLARE_ALLOCATOR( QuatLerp, AM_DEFAULT );

   public:
      void operator()( Quaternion& outQuat, const Quaternion& start, const Quaternion& end, const FastFloat& percentage ) const
      {
         outQuat.setSlerp( start, end, percentage );
      }
   };

private:
   std::string                                        m_boneName;
   float                                              m_duration;

   AnimationTimeline< Quaternion, QuatLerp >          m_orientation;
   AnimationTimeline< Vector, VecLerp >               m_translation;

public:
   /**
    * Constructor.
    *
    * @param boneName
    */
   BoneSRTAnimation( const std::string& boneName = "" );

   /**
    * Adds an animation orientation key definition at the specified time.
    *
    * @param frameTime
    * @param orientation
    */
   void addOrientationKey( float time, const Quaternion& orientation );

   /**
    * Adds an animation translation key definition at the specified time.
    *
    * @param frameTime
    * @param translation
    */
   void addTranslationKey( float time, const Vector& translation );

   /**
    * Returns the stream duration expressed in seconds.
    */
   inline float getDuration() const { return m_duration; }

   /**
    * Returns the bone name.
    */
   inline const std::string& getBoneName() const { return m_boneName; }

   // -------------------------------------------------------------------------
   // Raw data access
   // -------------------------------------------------------------------------
   /**
    * Returns the number of translation keys.
    */
   inline unsigned int getTranslationKeysCount() const { return m_translation.m_keys.size(); }

   /**
    * Returns a requested translation key.
    *
    * @param keyIdx     requested key index
    * @param outValue
    * @param outTime
    */
   void getTranslationKey( unsigned int keyIdx, Vector& outValue, float& outTime ) const;

   /**
    * Sets a value of a translation key.
    *
    * @param keyIdx     requested key index
    * @param value
    */
   void setTranslationKey( unsigned int keyIdx, const Vector& value );

   /**
    * Returns the number of orientation keys.
    */
   inline unsigned int getOrientationKeysCount() const { return m_orientation.m_keys.size(); }

   /**
    * Returns a requested orientation key.
    *
    * @param keyIdx     requested key index
    * @param outValue
    * @param outTime
    */
   void getOrientationKey( unsigned int keyIdx, Quaternion& outValue, float& outTime ) const;

   /**
    * Sets a value of an orientation key.
    *
    * @param keyIdx     requested key index
    * @param value
    */
   void setOrientationKey( unsigned int keyIdx, const Quaternion& value );

   // -------------------------------------------------------------------------
   // Serializable implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

private:
   // -------------------------------------------------------------------------
   // API for BoneSRTAnimationPlayer class
   // -------------------------------------------------------------------------
   friend class BoneSRTAnimationPlayer;

   /**
    * Returns the translation value at the specified time.
    *
    * @param lastCheckedKeyIdx
    * @param time
    * @param outTranslation
    *
    * @return  'true' if a value was found, 'false' otherwise
    */
   bool getTranslation( unsigned int& lastCheckedKeyIdx, float time, Vector& outTranslation ) const;

   /**
    * Returns the orientation value at the specified time.
    *
    * @param lastCheckedKeyIdx
    * @param time
    * @param outOrientation
    *
    * @return  'true' if a value was found, 'false' otherwise
    */
   bool getOrientation( unsigned int& lastCheckedKeyIdx, float time, Quaternion& outOrientation ) const;

private:
   void updateDuration();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A structure used by the animation definition for describing the animation
 * moment that's being currently played.
 */
class BoneSRTAnimationPlayer
{
private:
   const BoneSRTAnimation&       m_animation;

   unsigned int                  m_orientationKeyIdx;
   unsigned int                  m_translationKeyIdx;

public:
   /**
    * Constructor.
    *
    * @param animation     animation to play
    */
   BoneSRTAnimationPlayer( const BoneSRTAnimation& animation );

   /**
    * Returns the translation value at the specified time.
    *
    * @param time
    * @param outTranslation
    *
    * @return  'true' if a value was found, 'false' otherwise
    */
   bool getTranslation( float time, Vector& outTranslation );

   /**
    * Returns the orientation value at the specified time.
    *
    * @param time
    * @param outOrientation
    *
    * @return  'true' if a value was found, 'false' otherwise
    */
   bool getOrientation( float time, Quaternion& outOrientation );
};

///////////////////////////////////////////////////////////////////////////////
