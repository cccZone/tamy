#include "core-TestFramework\TestFramework.h"
#include "SoundMock.h"
#include "SoundChannelMock.h"


//////////////////////////////////////////////////////////////////////////////

TEST(SoundChannel, samplesUnloadedDuringSoundREmoval)
{
   SoundChannelMock channel;
   SoundMock sound;

   channel.assignSound(sound);

   CPPUNIT_ASSERT_EQUAL(true, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(0, channel.getActiveBuffersCount());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(1, channel.getActiveBuffersCount());

   channel.removeSound();
   CPPUNIT_ASSERT_EQUAL(false, channel.isBusy());
   CPPUNIT_ASSERT_EQUAL(0, channel.getActiveBuffersCount());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundChannel, playingSound)
{
   int soundLength = 6;
   int soundSampleLength = 2;
   SoundChannelMock channel;
   SoundMock sound(soundLength);

   channel.assignSound(sound);
   channel.setSampleLength(soundSampleLength);

   CPPUNIT_ASSERT_EQUAL((DWORD)0, sound.getDataOffset());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL((DWORD)2, sound.getDataOffset());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL((DWORD)4, sound.getDataOffset());

   // the sound is over - we stop playing (we stop reading from the buffer - the offset stays still)
   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL((DWORD)6, sound.getDataOffset());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL((DWORD)6, sound.getDataOffset());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundChannel, loopedPlaying)
{
   int soundLength = 6;
   DWORD soundSampleLength = 4;
   SoundChannelMock channel;
   SoundMock sound(soundLength);

   channel.assignSound(sound);

   channel.setSampleLength(soundSampleLength);
   channel.setLooped(true);

   CPPUNIT_ASSERT_EQUAL((DWORD)0, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL((DWORD)0, sound.getDataOffset());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(soundSampleLength, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL((DWORD)4, sound.getDataOffset());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(soundSampleLength, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL((DWORD)2, sound.getDataOffset());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(soundSampleLength, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL((DWORD)6, sound.getDataOffset());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(soundSampleLength, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL((DWORD)4, sound.getDataOffset());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundChannel, loopedPlayingWithSampleLargeEnoughToEncompassManyLoops)
{
   int soundLength = 3;
   DWORD soundSampleLength = 8;
   SoundChannelMock channel;
   SoundMock sound(soundLength);

   channel.assignSound(sound);

   channel.setSampleLength(soundSampleLength);
   channel.setLooped(true);

   CPPUNIT_ASSERT_EQUAL((DWORD)0, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL((DWORD)0, sound.getDataOffset());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(soundSampleLength, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL((DWORD)2, sound.getDataOffset());

   channel.loadNextSample();
   CPPUNIT_ASSERT_EQUAL(soundSampleLength, channel.getNumBytesToPlay());
   CPPUNIT_ASSERT_EQUAL((DWORD)1, sound.getDataOffset());
}

//////////////////////////////////////////////////////////////////////////////

TEST(SoundChannel, settingSoundAdjustsBufferSize)
{
   int soundLength = 3;
   DWORD bytesPerSec = 88200; // pick a num that's a little bit uneven bitwise
   DWORD blockAlignment = 4;
   DWORD soundSampleLength = 8;
   SoundChannelMock channel;
   SoundMock sound(soundLength, bytesPerSec, blockAlignment);

   CPPUNIT_ASSERT_EQUAL((DWORD)0xffff, channel.getSampleLength());

   channel.assignSound(sound);
   CPPUNIT_ASSERT_EQUAL((DWORD)22048, channel.getSampleLength()); // it's an even binary num
}

//////////////////////////////////////////////////////////////////////////////
