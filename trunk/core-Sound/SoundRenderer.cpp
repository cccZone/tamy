#include "core-Sound\SoundRenderer.h"
#include "core-Sound\SoundDevice.h"
#include "core-Sound\SoundSceneManager.h"
#include "core-Sound\SoundListener.h"
#include "core-Sound\SoundChannel.h"
#include "core-Sound\Sound3D.h"


///////////////////////////////////////////////////////////////////////////////

SoundRenderer::SoundRenderer(SoundDevice& soundDevice)
      : m_soundDevice(soundDevice)
{
}

///////////////////////////////////////////////////////////////////////////////

SoundRenderer::~SoundRenderer()
{
}

///////////////////////////////////////////////////////////////////////////////

void SoundRenderer::render(SoundSceneManager& soundScene)
{
   if (soundScene.hasActiveListener() == false) {return;}

   // update the listener
   SoundListener& listener = soundScene.getActiveListener();
   listener.update();


   soundScene.calculateSoundsHearability();
 
   DWORD soundsCount = 0;
   Sound3DP* sounds = NULL;

   // deactivate sounds that can no longer be heard
   sounds = soundScene.getSoundsToDisable(soundsCount);
   for (DWORD i = 0; i < soundsCount; ++i)
   {
      sounds[i]->deassignChannel();
   }

   // assign channels to active sounds
   sounds = soundScene.getSoundsToEnable(soundsCount);
   for (DWORD i = 0; i < soundsCount; ++i)
   {
      sounds[i]->assignChannel(m_soundDevice);
   }

   // render the sounds
   for (DWORD i = 0; i < soundsCount; ++i)
   {
      sounds[i]->update();
   }

   // update the device - effectively presenting the sounds
   m_soundDevice.update();
}

///////////////////////////////////////////////////////////////////////////////
