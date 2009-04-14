#pragma once

#include "core-Sound\SoundDevice.h"
#include "SoundChannelMock.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class SoundDeviceMock : public SoundDevice
{
private:
   std::vector<SoundChannelMock*> m_channels;

public:
   SoundDeviceMock(int channelsCount = 1, int numBuffersUsed = 1) 
      : SoundDevice(numBuffersUsed)
   {
      for (int i = 0; i < channelsCount; ++i)
      {
         m_channels.push_back(new SoundChannelMock());
      }
   }

   ~SoundDeviceMock()
   {
      for (unsigned int i = 0; i < m_channels.size(); ++i)
      {
         delete m_channels[i];
      }
      m_channels.clear();
   }

   int getChannelsCount() const
   {
      return m_channels.size();
   }


   SoundChannel& getChannel(int channelIdx)
   {
      return *(m_channels.at(channelIdx));
   }
};

///////////////////////////////////////////////////////////////////////////////
