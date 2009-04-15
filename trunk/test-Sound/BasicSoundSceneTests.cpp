#include "core-TestFramework\TestFramework.h"
#include "core\Node.h"
#include "core\MatrixWriter.h"
#include "core-Sound\BasicSoundSceneManager.h"
#include "SoundMock.h"
#include "Sound3DMock.h"
#include "SoundListenerMock.h"
#include "SoundDeviceMock.h"
#include <d3dx9.h>



//////////////////////////////////////////////////////////////////////////////

TEST(BasicSoundSceneManager, soundsAreHeardOnlyToCertainDistance)
{
   SoundDeviceMock soundDevice;
   BasicSoundSceneManager soundScene;

   SoundMock barking;
   float dogSoundHearingRadius = 5;
   float listenerHearingRadius = 1;
   Sound3DMock* dogSound = new Sound3DMock("dogSound", barking, dogSoundHearingRadius);
   
   Node dog("dog");
   dog.addChild(dogSound);

   soundScene.add(*dogSound);

   SoundListenerMock listener;
   soundScene.setActiveListener(listener);

   DWORD soundsToEnableCount = 0;   
   DWORD soundsToDisableCount = 0;
   Sound3DP* soundsToEnable = NULL;
   Sound3DP* soundsToDisable = NULL;


   soundScene.calculateSoundsHearability();
   soundsToEnable = soundScene.getSoundsToEnable(soundsToEnableCount);
   soundsToDisable = soundScene.getSoundsToDisable(soundsToDisableCount);
   CPPUNIT_ASSERT_EQUAL(dogSound->getGlobalMtx(), listener.getGlobalMtx());
   CPPUNIT_ASSERT_EQUAL((DWORD)1, soundsToEnableCount);
   CPPUNIT_ASSERT_EQUAL((DWORD)0, soundsToDisableCount);

   // assign a phony channel to the sound that's hearable
   soundsToEnable[0]->assignChannel(soundDevice);

   // no change in listener's position - no change in sounds hearability either
   soundScene.calculateSoundsHearability();
   soundsToEnable = soundScene.getSoundsToEnable(soundsToEnableCount);
   soundsToDisable = soundScene.getSoundsToDisable(soundsToDisableCount);
   CPPUNIT_ASSERT_EQUAL(dogSound->getGlobalMtx(), listener.getGlobalMtx());
   CPPUNIT_ASSERT_EQUAL((DWORD)0, soundsToEnableCount);
   CPPUNIT_ASSERT_EQUAL((DWORD)0, soundsToDisableCount);

   // we're completely outside the sound hearing are - no sound is heard
   D3DXMatrixTranslation(&(listener.accessLocalMtx()), dogSoundHearingRadius + 1, 0, 0);
   soundScene.calculateSoundsHearability();
   soundsToEnable = soundScene.getSoundsToEnable(soundsToEnableCount);
   soundsToDisable = soundScene.getSoundsToDisable(soundsToDisableCount);
   CPPUNIT_ASSERT_EQUAL((DWORD)0, soundsToEnableCount);
   CPPUNIT_ASSERT_EQUAL((DWORD)1, soundsToDisableCount);

   // deassign the channel from the sound that can no longer be heard
   soundsToDisable[0]->deassignChannel();


   D3DXMatrixTranslation(&(listener.accessLocalMtx()), dogSoundHearingRadius - 1, 0, 0);
   soundScene.calculateSoundsHearability();
   soundScene.getSoundsToEnable(soundsToEnableCount);
   soundScene.getSoundsToDisable(soundsToDisableCount);
   CPPUNIT_ASSERT_EQUAL((DWORD)1, soundsToEnableCount);
   CPPUNIT_ASSERT_EQUAL((DWORD)0, soundsToDisableCount);
}

//////////////////////////////////////////////////////////////////////////////
