#include "core-TestFramework\TestFramework.h"
#include "core\Node.h"
#include "core\MatrixWriter.h"
#include "core-Sound\SoundRenderer.h"
#include "SoundDeviceMock.h"
#include "SoundMock.h"
#include "Sound3DMock.h"
#include "SoundListenerMock.h"
#include "core-Sound\SoundSceneManager.h"
#include <d3dx9.h>


//////////////////////////////////////////////////////////////////////////////

TEST(SoundRenderer, renderingSound)
{
   SoundDeviceMock soundDevice;
   SoundRenderer soundRenderer(soundDevice);

   SoundSceneManager soundScene;

   SoundMock someSound;
   Sound3DMock* barkingSound = new Sound3DMock("barkingSound", someSound, 100);
   
   Node dog("dog", false);
   dog.addChild(barkingSound);
   soundScene.addNode(&dog);

   SoundListenerMock listener;
   soundScene.addNode(&listener);

   CPPUNIT_ASSERT_EQUAL(false, soundDevice.getChannel(0).isBusy());
   CPPUNIT_ASSERT_EQUAL(false, soundDevice.getChannel(0).isPlaying());

   soundRenderer.render(soundScene);

   CPPUNIT_ASSERT_EQUAL(true, soundDevice.getChannel(0).isBusy());
   CPPUNIT_ASSERT_EQUAL(true, soundDevice.getChannel(0).isPlaying());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundRenderer, soundPlayedContinuouslyAcrossRenderings)
{
   SoundDeviceMock soundDevice;
   SoundRenderer soundRenderer(soundDevice);

   SoundSceneManager soundScene;

   SoundMock someSound;
   Sound3DMock* barkingSound = new Sound3DMock("barkingSound", someSound, 100);
   
   Node dog("dog", false);
   dog.addChild(barkingSound);
   soundScene.addNode(&dog);

   SoundListenerMock listener;
   soundScene.addNode(&listener);

   SoundChannelMock& channel = dynamic_cast<SoundChannelMock&> (soundDevice.getChannel(0));

   CPPUNIT_ASSERT_EQUAL(false, channel.isBusy());

   soundRenderer.render(soundScene);
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.wasReassigned());

   soundRenderer.render(soundScene);
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(false, channel.wasReassigned());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundRenderer, channelsAreReleasedIfSoundCantBeHeardAnymore)
{
   SoundDeviceMock soundDevice;
   SoundRenderer soundRenderer(soundDevice);

   SoundSceneManager soundScene;

   SoundMock someSound;
   Sound3DMock* barkingSound = new Sound3DMock("barkingSound", someSound, 1);
   Sound3DMock* walkingSound = new Sound3DMock("walkingSound", someSound, 1);
   
   Node dog("dog", false);
   dog.addChild(barkingSound);
   D3DXMatrixTranslation(&(dog.accessLocalMtx()), -10, 0, 0);
   Node man("man", false);
   man.addChild(walkingSound);
   D3DXMatrixTranslation(&(man.accessLocalMtx()), 10, 0, 0);

   SoundListenerMock listener;
   soundScene.addNode(&listener);

   SoundChannelMock& channel = dynamic_cast<SoundChannelMock&> (soundDevice.getChannel(0));

   CPPUNIT_ASSERT_EQUAL(false, channel.isBusy());

   // first - the listener is near the man - he can hear his walking,
   // but he can't hear the dog barking
   D3DXMatrixTranslation(&(listener.accessLocalMtx()), 10, 0, 0);
   soundScene.addNode(&man);
   soundRenderer.render(soundScene);
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());

   // next moment - the listener moves close to the dog - he can hear it barking,
   // but he can't hear the man walking
   D3DXMatrixTranslation(&(listener.accessLocalMtx()), -10, 0, 0);
   soundScene.addNode(&dog);
   soundRenderer.render(soundScene);
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.wasReassigned());
}

//////////////////////////////////////////////////////////////////////////////
