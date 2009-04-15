#pragma once

#include "core-Sound\SoundSceneManager.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a basic implementation of a sound scene manager
 */
class BasicSoundSceneManager : public SoundSceneManager
{
private:
   SoundListener* m_activeListener;

   DWORD m_soundsCount;
   DWORD m_soundsArraySize;
   Sound3DP* m_soundsArr;

   DWORD m_soundsToDisableCount;
   Sound3DP* m_soundsToDisableArr;

   DWORD m_soundsToEnableCount;
   Sound3DP* m_soundsToEnableArr;

public:
   BasicSoundSceneManager();
   ~BasicSoundSceneManager();

   void setActiveListener(SoundListener& listener);
   bool hasActiveListener() const;
   SoundListener& getActiveListener();

   void add(Sound3D& sound);

   void calculateSoundsHearability();
   Sound3DP* getSoundsToDisable(DWORD& soundsCount);
   Sound3DP* getSoundsToEnable(DWORD& soundsCount);
};

///////////////////////////////////////////////////////////////////////////////
