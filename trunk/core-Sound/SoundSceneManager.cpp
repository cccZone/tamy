#include "core-Sound\SoundSceneManager.h"
#include "core\Node.h"
#include "core-Sound\SoundListener.h"
#include "core-Sound\Sound3D.h"


///////////////////////////////////////////////////////////////////////////////

SoundSceneManager::SoundSceneManager()
      : m_activeListener(NULL)
{
   REGISTER_SCENE_ASPECT(SoundListener);
   REGISTER_SCENE_ASPECT(Sound3D);
}

///////////////////////////////////////////////////////////////////////////////

SoundSceneManager::~SoundSceneManager()
{
}

///////////////////////////////////////////////////////////////////////////////

void SoundSceneManager::add(SoundListener& listener)
{
   m_activeListener = &listener;
}

///////////////////////////////////////////////////////////////////////////////

void SoundSceneManager::remove(SoundListener& listener)
{
   m_activeListener = NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool SoundSceneManager::hasActiveListener() const
{
   return (m_activeListener != NULL);
}

///////////////////////////////////////////////////////////////////////////////

SoundListener& SoundSceneManager::getActiveListener()
{
   if (m_activeListener == NULL)
   {
      throw std::logic_error("No active listener set");
   }
   return *m_activeListener;
}

///////////////////////////////////////////////////////////////////////////////

void SoundSceneManager::add(Sound3D& sound)
{
   m_soundsArr.push_back(&sound);
}

///////////////////////////////////////////////////////////////////////////////

void SoundSceneManager::remove(Sound3D& sound)
{
   unsigned int idx = m_soundsArr.find(&sound);
   m_soundsArr.remove(idx);
}

///////////////////////////////////////////////////////////////////////////////

Array<Sound3D*>& SoundSceneManager::update(Array<Sound3D*>& soundsToDisable, 
                                           Array<Sound3D*>& soundsToEnable)
{
   if (m_activeListener == NULL) {return m_soundsArr;}

   D3DXMATRIX listenerMtx = m_activeListener->getGlobalMtx();
   D3DXVECTOR3 listenerPos(listenerMtx._41, listenerMtx._42, listenerMtx._43);
   D3DXMATRIX soundMtx;
   D3DXVECTOR3 distVec;

   Sound3D* sound = NULL;
   DWORD soundsCount = m_soundsArr.size();
   for (DWORD i = 0; i < soundsCount; ++i)
   {
      soundMtx = m_soundsArr[i]->getGlobalMtx();
      distVec = listenerPos - D3DXVECTOR3(soundMtx._41, soundMtx._42, soundMtx._43);

      sound = m_soundsArr[i];

      bool hearable = (D3DXVec3LengthSq(&distVec) <= m_soundsArr[i]->getHearingRadiusSq());
      bool active = sound->hasChannelAssigned();

      if ((hearable == true) && (active == false))
      {
         soundsToEnable.push_back(sound);
      }
      else if ((hearable == false) && (active == true))
      {
         soundsToDisable.push_back(sound);
      }
   }

   return m_soundsArr;
}

///////////////////////////////////////////////////////////////////////////////
