#include "core-Sound\SoundChannel.h"
#include "core-Sound\Sound.h"
#include <stdexcept>
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

SoundChannel::SoundChannel(int id)
      : m_id(id),
      m_sound(NULL),
      m_sampleLength(0xffff),
      m_isPlaying(false),
      m_looped(false),
      m_currSoundFreq(0),
      m_currPeriodicPos(0),
      m_soundLength(0)
{
   m_data = new char[m_sampleLength];
}

///////////////////////////////////////////////////////////////////////////////

SoundChannel::~SoundChannel()
{
   m_sound = NULL;

   delete [] m_data;
   m_data = NULL;

   m_sampleLength = 0;

   m_isPlaying = false;
   m_looped = false;
   m_currSoundFreq = 0;

   m_currPeriodicPos = 0;
   m_soundLength = 0;
}

///////////////////////////////////////////////////////////////////////////////

void SoundChannel::loadNextSample()
{
   if (m_sound == NULL) {return;}

   DWORD len = m_sampleLength;
   DWORD offset = 0;

   DWORD bytesRead; 
   while ((len > 0) && (m_currPeriodicPos < m_soundLength))
   {
      bytesRead = m_sound->getData(m_currPeriodicPos, m_data + offset, len);
      len -= bytesRead;
      offset += bytesRead;
      setPosition(m_currPeriodicPos + bytesRead);
   };

   if (offset > 0)
   {
      addDataToPlayBuffer(m_data, m_sampleLength, m_currSoundFormat, m_currSoundFreq);
   }
}

///////////////////////////////////////////////////////////////////////////////

void SoundChannel::play()
{
   m_isPlaying = true;
   startPlaying();
}

///////////////////////////////////////////////////////////////////////////////

void SoundChannel::stop()
{
   stopPlaying();
   m_isPlaying = false;
}

///////////////////////////////////////////////////////////////////////////////

bool SoundChannel::isBusy() const
{
   return m_sound != NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool SoundChannel::isPlaying() const
{
   return m_isPlaying;
}

///////////////////////////////////////////////////////////////////////////////

void SoundChannel::assignSound(Sound& sound)
{
   m_sound = &sound;

   DWORD newBufSize = m_sound->getBytesPerSec();/* >> 2;*/ //TODO
   newBufSize -= newBufSize % m_sound->getBlockAlignment();
   setSampleLength(newBufSize);
   
   m_currSoundFormat = m_sound->getFormat();
   m_currSoundFreq = m_sound->getFrequency();

   m_soundLength = m_sound->getLength();
   m_currPeriodicPos = 0;
}

///////////////////////////////////////////////////////////////////////////////

void SoundChannel::removeSound()
{
   stop();
   cleanBuffers();
   m_sound = NULL;
   m_currSoundFormat = "";
   m_currSoundFreq = 0;
   m_soundLength = 0;
   m_currPeriodicPos = 0;
}

///////////////////////////////////////////////////////////////////////////////

void SoundChannel::setSampleLength(DWORD len)
{
   delete [] m_data;

   m_sampleLength = len;
   m_data = new char[m_sampleLength];
}

///////////////////////////////////////////////////////////////////////////////

DWORD SoundChannel::getSampleLength() const
{
   return m_sampleLength;
}

///////////////////////////////////////////////////////////////////////////////

void SoundChannel::setLooped(bool enable)
{
   m_looped = enable;
}

///////////////////////////////////////////////////////////////////////////////

float SoundChannel::getPosition() const
{
   if (m_sound == NULL) {return 0.f;}
   return (float)m_currPeriodicPos / (float)m_sound->getBytesPerSec();
}

///////////////////////////////////////////////////////////////////////////////

void SoundChannel::setPosition(float pos)
{
   if (m_sound == NULL) {return;}
   m_currPeriodicPos = (DWORD)(pos * (float)m_sound->getBytesPerSec()) % m_soundLength;
}

///////////////////////////////////////////////////////////////////////////////

void SoundChannel::setPosition(DWORD pos)
{
   if (m_looped)
   {
      m_currPeriodicPos = pos % m_soundLength;
   }
   else
   {
      m_currPeriodicPos = pos;
      if (m_currPeriodicPos > m_soundLength) {m_currPeriodicPos = m_soundLength;}
   }
}

///////////////////////////////////////////////////////////////////////////////

float SoundChannel::getSoundLength() const
{
   if (m_sound == NULL) {return 0.f;}
   return (float)m_soundLength / (float)m_sound->getBytesPerSec();
}

///////////////////////////////////////////////////////////////////////////////
