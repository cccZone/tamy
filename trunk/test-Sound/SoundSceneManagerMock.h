#pragma once

#include "core-Sound\SoundSceneManager.h"


///////////////////////////////////////////////////////////////////////////////

class SoundSceneManagerMock : public SoundSceneManager
{
private:
   SoundListener* m_listener;
   Sound3D* m_oldSound;
   Sound3D* m_sound;
   Sound3D* m_newSound;
   Sound3D* m_currSound;

public:
   SoundSceneManagerMock() 
      : m_listener(NULL), 
      m_oldSound(NULL), 
      m_sound(NULL), 
      m_newSound(NULL),
      m_currSound(NULL)
   {}

   void setActiveListener(SoundListener& listener) 
   {
      m_listener = &listener;
   }

   bool hasActiveListener() const
   {
      return m_listener != NULL;
   }

   SoundListener& getActiveListener()
   {
      return *m_listener;
   }

   void add(Sound3D& sound)
   {
      if (m_currSound != NULL)
      {
         m_oldSound = m_currSound;
      }

      m_sound = &sound;
   }

   void calculateSoundsHearability()
   {
      if (m_newSound != NULL)
      {
         m_newSound = NULL;
      }

      if (m_sound != NULL)
      {
         m_newSound = m_sound;
         m_sound = NULL;
      }
   }

   Sound3DP* getSoundsToDisable(DWORD& soundsCount)
   {
      if (m_oldSound == NULL)
      {
         soundsCount = 0;
         return NULL;
      }
      else
      {
         m_currSound = NULL;
         soundsCount = 1;
         return &m_oldSound;
      }
   }

   Sound3DP* getSoundsToEnable(DWORD& soundsCount)
   {
      if (m_newSound == NULL)
      {
         soundsCount = 0;
         return NULL;
      }
      else
      {
         m_currSound = m_newSound;
         soundsCount = 1;
         return &m_newSound;
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
