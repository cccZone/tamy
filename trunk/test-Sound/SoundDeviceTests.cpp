#include "core-TestFramework\TestFramework.h"
#include "SoundDeviceMock.h"
#include "SoundMock.h"
#include "SoundChannelMock.h"


//////////////////////////////////////////////////////////////////////////////

TEST(SoundDevice, removingSoundThatFinishedPlaying)
{
   SoundDeviceMock soundDevice;
   int soundLength = 2;
   SoundMock sound(soundLength);

   SoundChannelMock& channel = dynamic_cast<SoundChannelMock&> (soundDevice.activateSound(sound));

   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(0, channel.getActiveBuffersCount());

   channel.play();
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(0, channel.getActiveBuffersCount());

   soundDevice.update(); 
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());

   channel.simulateBufferEnd();

   soundDevice.update(); 
   CPPUNIT_ASSERT_EQUAL(false, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(0, channel.getActiveBuffersCount());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundDevice, continuousPlayingOfLoopedSound)
{
   SoundDeviceMock soundDevice;
   int soundLength = 2;
   SoundMock sound(soundLength);

   SoundChannelMock& channel = dynamic_cast<SoundChannelMock&> (soundDevice.activateSound(sound));
   channel.setLooped(true);


   channel.play();
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(0, channel.getActiveBuffersCount());

   soundDevice.update(); 
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());

   channel.simulateBufferEnd();

   soundDevice.update();  // buffer gets reloaded
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());

   channel.simulateBufferEnd();

   soundDevice.update();  // ... and so on
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());

   channel.setLooped(false);
   channel.simulateBufferEnd();

   soundDevice.update(); // unitl we turn the looping off - after that we play the sound only once more ...
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());

   channel.simulateBufferEnd();
   soundDevice.update(); // ... but that's it
   CPPUNIT_ASSERT_EQUAL(false, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(0, channel.getActiveBuffersCount());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundDevice, stoppingTheSoundAllowsToResumeIt)
{
   SoundDeviceMock soundDevice;
   int soundLength = 2;
   SoundMock sound(soundLength);

   SoundChannelMock& channel = dynamic_cast<SoundChannelMock&> (soundDevice.activateSound(sound));
   channel.setLooped(true);


   channel.play();
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(0, channel.getActiveBuffersCount());

   soundDevice.update(); 
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());

   channel.simulateBufferEnd();

   soundDevice.update();  // buffer gets reloaded
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());

   channel.stop();
   soundDevice.update();  // buffer gets reloaded
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(false, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());

   channel.play();
   soundDevice.update();
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundDevice, makingSureChannelDoesntRunOutOfData)
{
   int channelsCount = 1;
   int numBuffersUsed = 3;
   SoundDeviceMock soundDevice(channelsCount, numBuffersUsed);
   int soundLength = 8;
   int soundSampleLength = 2;
   SoundMock sound(soundLength);

   SoundChannelMock& channel = dynamic_cast<SoundChannelMock&> (soundDevice.activateSound(sound));
   channel.setLooped(false);
   channel.setSampleLength(soundSampleLength);

   channel.play();
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(0, channel.getActiveBuffersCount());

   soundDevice.update(); 
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());

   soundDevice.update(); 
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(2, channel.getActiveBuffersCount());

   soundDevice.update(); 
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(3, channel.getActiveBuffersCount());

   channel.simulateBufferEnd();

   soundDevice.update();  // buffer gets reloaded
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(3, channel.getActiveBuffersCount());

   channel.simulateBufferEnd();

   soundDevice.update();  // that was the last chunk opf data we could get - now we simply run out of data...
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(2, channel.getActiveBuffersCount());

   channel.simulateBufferEnd();

   soundDevice.update();
   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(true, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());

   channel.simulateBufferEnd();

   soundDevice.update();
   CPPUNIT_ASSERT_EQUAL(false, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(false, channel.isPlaying());
   CPPUNIT_ASSERT_EQUAL(0, channel.getActiveBuffersCount());
}

//////////////////////////////////////////////////////////////////////////////
