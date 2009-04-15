#include "core-Sound\SoundDevice.h"
#include "core-Sound\Sound.h"
#include "core-Sound\SoundChannel.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

SoundDevice::SoundDevice(int numBuffersUsed)
      : m_numBuffersUsed(numBuffersUsed)
{
}

///////////////////////////////////////////////////////////////////////////////

SoundDevice::~SoundDevice() 
{
}

///////////////////////////////////////////////////////////////////////////////

SoundChannel& SoundDevice::activateSound(Sound& sound)
{
   int freeChannel = -1;
   int channelsCount = getChannelsCount();
   for (int i = 0; i < channelsCount; ++i)
   {
      if (getChannel(i).isBusy() == false)
      {
         freeChannel = i;
         break;
      }
   }

   if (freeChannel == -1)
   {
      throw std::runtime_error("All sound channels are currently occupied");
   }

   SoundChannel& channel = getChannel(freeChannel);
   m_activeChannels.push_back(&channel);
   channel.assignSound(sound);
   return channel;
}

///////////////////////////////////////////////////////////////////////////////

void SoundDevice::releaseAllChannels()
{
   while(m_activeChannels.size() > 0)
   {
      SoundChannel& channel = *(m_activeChannels.front());
      channel.stop();
      channel.removeSound();
      m_activeChannels.pop_front();
   }
}

///////////////////////////////////////////////////////////////////////////////

void SoundDevice::update()
{
   std::list<SoundChannel*>::iterator it = m_activeChannels.begin();
   while(it != m_activeChannels.end())
   {
      SoundChannel& channel = **it;
      if (channel.isPlaying() == false) {++it; continue;}

      channel.update();
      if (channel.getActiveBuffersCount() < m_numBuffersUsed) {channel.loadNextSample();}
      if (channel.getActiveBuffersCount() == 0)
      {
         channel.removeSound();
         it = m_activeChannels.erase(it);
      }
      else
      {
         ++it;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
