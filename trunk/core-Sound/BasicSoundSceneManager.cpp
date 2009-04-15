#include "core-Sound\BasicSoundSceneManager.h"
#include "core-Sound\Sound3D.h"
#include "core-Sound\SoundListener.h"
#include <stdexcept>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BasicSoundSceneManager::BasicSoundSceneManager()
      : m_activeListener(NULL)
{
   m_soundsCount = 0;
   m_soundsToDisableCount = 0;
   m_soundsToEnableCount = 0;
   m_soundsArraySize = 1;
   m_soundsArr = new Sound3DP[m_soundsArraySize];
   m_soundsToDisableArr = new Sound3DP[m_soundsArraySize];
   m_soundsToEnableArr = new Sound3DP[m_soundsArraySize];
}

///////////////////////////////////////////////////////////////////////////////

BasicSoundSceneManager::~BasicSoundSceneManager() 
{
   delete [] m_soundsToDisableArr;
   m_soundsToDisableArr = NULL;

   delete [] m_soundsToEnableArr;
   m_soundsToEnableArr = NULL;

   delete [] m_soundsArr;
   m_soundsArr = NULL;

   m_soundsArraySize = 0;
   m_soundsCount = 1;
}

///////////////////////////////////////////////////////////////////////////////

void BasicSoundSceneManager::setActiveListener(SoundListener& listener)
{
   m_activeListener = &listener;
}

///////////////////////////////////////////////////////////////////////////////

bool BasicSoundSceneManager::hasActiveListener() const
{
   return (m_activeListener != NULL);
}

///////////////////////////////////////////////////////////////////////////////

SoundListener& BasicSoundSceneManager::getActiveListener()
{
   if (m_activeListener == NULL)
   {
      throw std::logic_error("No active listener set");
   }
   return *m_activeListener;
}

///////////////////////////////////////////////////////////////////////////////

void BasicSoundSceneManager::add(Sound3D& sound)
{
   if (m_soundsCount + 1 >  m_soundsArraySize)
   {
      // resize the array that keeps the track of all sounds
      DWORD newSize = m_soundsArraySize * 2;
      Sound3DP* tmpArr = new Sound3DP[newSize];
      ZeroMemory(tmpArr, sizeof(Sound3DP) * newSize);

      memcpy(tmpArr, m_soundsArr, 
             sizeof(Sound3DP) * m_soundsArraySize);
      delete [] m_soundsArr;

      m_soundsArraySize = newSize;
      m_soundsArr = tmpArr;

      // resize the array used to keep track of the sounds we can hear from the listener's position
      delete [] m_soundsToDisableArr;
      m_soundsToDisableArr = new Sound3DP[m_soundsArraySize];

      delete [] m_soundsToEnableArr;
      m_soundsToEnableArr = new Sound3DP[m_soundsArraySize];
   }

   m_soundsArr[m_soundsCount++] = &sound;
}

///////////////////////////////////////////////////////////////////////////////

void BasicSoundSceneManager::calculateSoundsHearability()
{
   m_soundsToDisableCount = 0;
   m_soundsToEnableCount = 0;
   if (m_activeListener == NULL) {return;}

   D3DXMATRIX listenerMtx = m_activeListener->getGlobalMtx();
   D3DXVECTOR3 listenerPos(listenerMtx._41, listenerMtx._42, listenerMtx._43);
   D3DXMATRIX soundMtx;
   D3DXVECTOR3 distVec;

   Sound3D* sound = NULL;
   for (DWORD i = 0; i < m_soundsCount; ++i)
   {
      soundMtx = m_soundsArr[i]->getGlobalMtx();
      distVec = listenerPos - D3DXVECTOR3(soundMtx._41, soundMtx._42, soundMtx._43);

      sound = m_soundsArr[i];

      bool hearable = (D3DXVec3LengthSq(&distVec) <= m_soundsArr[i]->getHearingRadiusSq());
      bool active = sound->hasChannelAssigned();
    
      if ((hearable == true) && (active == false))
      {
         m_soundsToEnableArr[m_soundsToEnableCount] = sound;
         m_soundsToEnableCount++;
      }
      else if ((hearable == false) && (active == true))
      {
         m_soundsToDisableArr[m_soundsToDisableCount] = sound;
         m_soundsToDisableCount++;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

Sound3DP* BasicSoundSceneManager::getSoundsToDisable(DWORD& soundsCount)
{
   soundsCount = m_soundsToDisableCount;
   return m_soundsToDisableArr;
}

///////////////////////////////////////////////////////////////////////////////

Sound3DP* BasicSoundSceneManager::getSoundsToEnable(DWORD& soundsCount)
{
   soundsCount = m_soundsToEnableCount;
   return m_soundsToEnableArr;
}

///////////////////////////////////////////////////////////////////////////////
