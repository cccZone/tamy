/// @file   core-AI/BoneSRTAnimation.h
/// @brief  animation data stream for a single bone
#pragma once

#include "core/Object.h"
#include "core-AI/AnimationTimeline.h"
#include <string>
#include <vector>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class BoneSRTAnimationPlayer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Animation data stream for a single bone.
 */
class BoneSRTAnimation : public Object
{
   DECLARE_CLASS( BoneSRTAnimation );

private:
   struct VecLerp
   {
   public:
      void operator()( D3DXVECTOR3& outVec, const D3DXVECTOR3& start, const D3DXVECTOR3& end, float percentage ) const
      {
         D3DXVec3Lerp( &outVec, &start, &end, percentage );
      }
   };

   struct QuatLerp
   {
   public:
      void operator()( D3DXQUATERNION& outQuat, const D3DXQUATERNION& start, const D3DXQUATERNION& end, float percentage ) const
      {
         D3DXQuaternionSlerp( &outQuat, &start, &end, percentage );
      }
   };

private:
   std::string                                        m_boneName;
   float                                              m_duration;

   AnimationTimeline< D3DXQUATERNION, QuatLerp >      m_orientation;
   AnimationTimeline< D3DXVECTOR3, VecLerp >          m_translation;

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
   void addOrientationKey( float time, const D3DXQUATERNION& orientation );

   /**
    * Adds an animation translation key definition at the specified time.
    *
    * @param frameTime
    * @param translation
    */
   void addTranslationKey( float time, const D3DXVECTOR3& translation );

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
   void getTranslationKey( unsigned int keyIdx, D3DXVECTOR3& outValue, float& outTime ) const;

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
   void getOrientationKey( unsigned int keyIdx, D3DXQUATERNION& outValue, float& outTime ) const;

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
   bool getTranslation( unsigned int& lastCheckedKeyIdx, float time, D3DXVECTOR3& outTranslation ) const;

   /**
    * Returns the orientation value at the specified time.
    *
    * @param lastCheckedKeyIdx
    * @param time
    * @param outOrientation
    *
    * @return  'true' if a value was found, 'false' otherwise
    */
   bool getOrientation( unsigned int& lastCheckedKeyIdx, float time, D3DXQUATERNION& outOrientation ) const;

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
   bool getTranslation( float time, D3DXVECTOR3& outTranslation );

   /**
    * Returns the orientation value at the specified time.
    *
    * @param time
    * @param outOrientation
    *
    * @return  'true' if a value was found, 'false' otherwise
    */
   bool getOrientation( float time, D3DXQUATERNION& outOrientation );
};

///////////////////////////////////////////////////////////////////////////////
