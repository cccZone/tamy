#include "core-TestFramework\TestFramework.h"
#include "core\Node.h"
#include "core\MatrixWriter.h"
#include "core-Sound\SoundRenderer.h"
#include "SoundDeviceMock.h"
#include "SoundMock.h"
#include "Sound3DMock.h"
#include "SoundListenerMock.h"
#include "SoundSceneManagerMock.h"
#include <d3dx9.h>


//////////////////////////////////////////////////////////////////////////////

TEST(SoundRenderer, renderingSound)
{
   SoundDeviceMock soundDevice;
   SoundRenderer soundRenderer(soundDevice);

   SoundSceneManagerMock soundScene;

   SoundMock someSound;
   Sound3DMock* barkingSound = new Sound3DMock("barkingSound", someSound, 100);
   
   Node dog("dog");
   dog.addChild(barkingSound);

   soundScene.add(*barkingSound);

   SoundListenerMock listener;
   soundScene.setActiveListener(listener);

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

   SoundSceneManagerMock soundScene;

   SoundMock someSound;
   Sound3DMock* barkingSound = new Sound3DMock("barkingSound", someSound, 100);
   
   Node dog("dog");
   dog.addChild(barkingSound);

   soundScene.add(*barkingSound);

   SoundListenerMock listener;
   soundScene.setActiveListener(listener);

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

   SoundSceneManagerMock soundScene;

   SoundMock someSound;
   Sound3DMock* barkingSound = new Sound3DMock("barkingSound", someSound, 1);
   Sound3DMock* walkingSound = new Sound3DMock("walkingSound", someSound, 1);
   
   Node dog("dog");
   dog.addChild(barkingSound);
   Node man("man");
   man.addChild(walkingSound);

 
   soundScene.add(*walkingSound);

   SoundListenerMock listener;
   soundScene.setActiveListener(listener);

   SoundChannelMock& channel = dynamic_cast<SoundChannelMock&> (soundDevice.getChannel(0));

   CPPUNIT_ASSERT_EQUAL(false, channel.isBusy());

   // first - the listener is near the man - he can hear his walking,
   // but he can't hear the dog barking
   soundScene.add(*walkingSound);
   soundRenderer.render(soundScene);
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());

   // next moment - the listener moves close to the dog - he can hear it barking,
   // but he can't hear the man walking
   soundScene.add(*barkingSound);
   soundRenderer.render(soundScene);
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.wasReassigned());
}

//////////////////////////////////////////////////////////////////////////////
