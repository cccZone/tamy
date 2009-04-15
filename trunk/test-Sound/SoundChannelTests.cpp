#include "core-TestFramework\TestFramework.h"
#include "SoundMock.h"
#include "SoundChannelMock.h"


//////////////////////////////////////////////////////////////////////////////

TEST(SoundChannel, samplesUnloadedDuringSoundRemoval)
{
   int soundLength = 2;
   int bytesPerSec = 1;
   SoundChannelMock channel(0);
   SoundMock sound(soundLength, bytesPerSec);

   channel.assignSound(sound);

   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(0, channel.getActiveBuffersCount());
   CPPUNIT_ASSERT_EQUAL(0.f, channel.getPosition());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());
   CPPUNIT_ASSERT_EQUAL(1.f, channel.getPosition());

   channel.removeSound();
   CPPUNIT_ASSERT_EQUAL(false, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(0, channel.getActiveBuffersCount());
   CPPUNIT_ASSERT_EQUAL(0.f, channel.getPosition());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundChannel, playingSound)
{
   int soundLength = 6;
   int bytesPerSec = 1;
   int soundSampleLength = 2;
   SoundChannelMock channel(0);
   SoundMock sound(soundLength, bytesPerSec);

   channel.assignSound(sound);
   channel.setSampleLength(soundSampleLength);

   CPPUNIT_ASSERT_EQUAL(0.f, channel.getPosition());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(2.f, channel.getPosition());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(4.f, channel.getPosition());

   // the sound is over - we stop playing (we stop reading from the buffer - the offset stays still)
   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(6.f, channel.getPosition());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(6.f, channel.getPosition());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundChannel, loopedPlaying)
{
   int soundLength = 6;
   int bytesPerSec = 1;
   DWORD soundSampleLength = 4;
   SoundChannelMock channel(0);
   SoundMock sound(soundLength, bytesPerSec);

   channel.assignSound(sound);

   channel.setSampleLength(soundSampleLength);
   channel.setLooped(true);

   CPPUNIT_ASSERT_EQUAL((DWORD)0, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL(0.f, channel.getPosition());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(soundSampleLength, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL(4.f, channel.getPosition());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(soundSampleLength, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL(2.f, channel.getPosition());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(soundSampleLength, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL(0.f, channel.getPosition());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(soundSampleLength, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL(4.f, channel.getPosition());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundChannel, loopedPlayingWithSampleLargeEnoughToEncompassManyLoops)
{
   int soundLength = 3;
   int bytesPerSec = 1;
   DWORD soundSampleLength = 8;
   SoundChannelMock channel(0);
   SoundMock sound(soundLength, bytesPerSec);

   channel.assignSound(sound);

   channel.setSampleLength(soundSampleLength);
   channel.setLooped(true);

   CPPUNIT_ASSERT_EQUAL((DWORD)0, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL(0.f, channel.getPosition());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(soundSampleLength, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL(2.f, channel.getPosition());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(soundSampleLength, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL(1.f, channel.getPosition());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundChannel, settingSoundAdjustsBufferSize)
{
   int soundLength = 3;
   DWORD bytesPerSec = 88200; // pick a num that's a little bit uneven bitwise
   DWORD blockAlignment = 4;
   DWORD soundSampleLength = 8;
   SoundChannelMock channel(0);
   SoundMock sound(soundLength, bytesPerSec, blockAlignment);

   CPPUNIT_ASSERT_EQUAL((DWORD)0xffff, channel.getSampleLength());

   channel.assignSound(sound);
   CPPUNIT_ASSERT_EQUAL((DWORD)88200, channel.getSampleLength()); // it's an even binary num
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundChannel, soundInstancing)
{
   int soundLength = 4;
   int bytesPerSec = 1;
   DWORD soundSampleLength = 1;
   SoundMock sound(soundLength, bytesPerSec);

   SoundChannelMock channel1(0);
   channel1.setSampleLength(soundSampleLength);
   SoundChannelMock channel2(1);
   channel2.setSampleLength(soundSampleLength);

   channel1.assignSound(sound);
   channel2.assignSound(sound);

   CPPUNIT_ASSERT_EQUAL(0.f, channel1.getPosition());
   CPPUNIT_ASSERT_EQUAL(0.f, channel2.getPosition());

   channel1.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(1.f, channel1.getPosition());
   CPPUNIT_ASSERT_EQUAL(0.f, channel2.getPosition());

   channel2.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(1.f, channel1.getPosition());
   CPPUNIT_ASSERT_EQUAL(1.f, channel2.getPosition());
}

//////////////////////////////////////////////////////////////////////////////
