#pragma once

#include "core-Sound\SoundDevice.h"
#include "impl-DirectX\SoundDeviceInfo.h"
#include <al.h>
#include <alc.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class OpenALSoundSystem;
class OALSoundChannel;

///////////////////////////////////////////////////////////////////////////////

class OALSoundDevice : public SoundDevice
{
private:
   SoundDeviceInfo m_deviceDesc;
   OpenALSoundSystem& m_soundSystem;
   ALCdevice* m_device;
   ALCcontext* m_context;
   std::vector<OALSoundChannel*> m_channels;

public:
   OALSoundDevice(const SoundDeviceInfo& deviceDesc,
                  OpenALSoundSystem& soundSystem,
                  int numBuffersUsed = 1);
   ~OALSoundDevice();

   int getChannelsCount() const;

   SoundChannel& getChannel(int channelIdx);
};

///////////////////////////////////////////////////////////////////////////////
