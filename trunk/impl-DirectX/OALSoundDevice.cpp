#include "impl-DirectX\OALSoundDevice.h"
#include "impl-DirectX\OpenALSoundSystem.h"
#include "impl-DirectX\OALSoundChannel.h"
#include <stdexcept>
#include <string>


///////////////////////////////////////////////////////////////////////////////

OALSoundDevice::OALSoundDevice(const SoundDeviceInfo& deviceDesc,
                               OpenALSoundSystem& soundSystem,
                               int numBuffersUsed)
      : SoundDevice(numBuffersUsed),
      m_deviceDesc(deviceDesc),
      m_soundSystem(soundSystem)
{
   if (deviceDesc.name.length() == 0)
   {
      throw std::invalid_argument("No name of device to be created specified");
   }

   m_device = m_soundSystem.alcOpenDevice(deviceDesc.name.c_str());
   if (m_device == NULL)
   {
      throw std::runtime_error(std::string("Cannot create sound device ") + deviceDesc.name);
   }

   m_context = m_soundSystem.alcCreateContext(m_device, NULL);
   if (m_context == NULL)
   {
	   throw std::runtime_error(std::string("Cannot create a context for sound device ") + deviceDesc.name);
   }
	m_soundSystem.alcMakeContextCurrent(m_context);

   for (unsigned int i = 0; i < m_deviceDesc.sourceCount; ++i)
   {
      m_channels.push_back(new OALSoundChannel(i, m_soundSystem, numBuffersUsed));
   }
}

///////////////////////////////////////////////////////////////////////////////

OALSoundDevice::~OALSoundDevice()
{
   for (unsigned int i = 0; i < m_channels.size(); ++i)
   {
      delete m_channels[i];
   }
   m_channels.clear();

   m_soundSystem.alcMakeContextCurrent(NULL);
	m_soundSystem.alcDestroyContext(m_context);
   m_soundSystem.alcCloseDevice(m_device);
   m_device = NULL;
}

///////////////////////////////////////////////////////////////////////////////

int OALSoundDevice::getChannelsCount() const
{
   return (int)m_channels.size();
}

///////////////////////////////////////////////////////////////////////////////

SoundChannel& OALSoundDevice::getChannel(int channelIdx)
{
   return *(m_channels.at(channelIdx));
}

///////////////////////////////////////////////////////////////////////////////
