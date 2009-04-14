#include "core-Sound\SoundChannel.h"
#include "core-Sound\Sound.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

SoundChannel::SoundChannel()
      : m_sound(NULL),
      m_sampleLength(0xffff),
      m_isPlaying(false),
      m_looped(false),
      m_currSoundFreq(0)
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
}

///////////////////////////////////////////////////////////////////////////////

void SoundChannel::loadNextSample()
{
   if (m_sound == NULL) {return;}

   if (m_looped == false)
   {
      DWORD bytesRead = m_sound->getData(m_data, m_sampleLength);

      if (bytesRead > 0)
      {
         addDataToPlayBuffer(m_data, bytesRead, m_currSoundFormat, m_currSoundFreq);
      }
      else
      {
         stopPlaying();
      }
   }
   else
   {
      DWORD len = m_sampleLength;
      DWORD offset = 0;

      DWORD bytesRead;
      
      do
      {
         bytesRead = m_sound->getData(m_data + offset, len);
         len -= bytesRead;
         offset += bytesRead;

         if (len > 0)
         {
            m_sound->setDataOffset(0);
         }
      } while(len > 0);

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

   DWORD newBufSize = m_sound->getBytesPerSec() >> 2;
   newBufSize -= newBufSize % m_sound->getBlockAlignment();
   setSampleLength(newBufSize);

   
   m_currSoundFormat = m_sound->getFormat();
   m_currSoundFreq = m_sound->getFrequency();
}

///////////////////////////////////////////////////////////////////////////////

void SoundChannel::removeSound()
{
   stop();
   cleanBuffers();
   m_sound = NULL;
   m_currSoundFormat = "";
   m_currSoundFreq = 0;
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

int SoundChannel::getActiveBuffersCount() const
{
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
