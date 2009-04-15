#include "core-Sound\Sound3D.h"
#include "core-Sound\Sound.h"
#include "core-Sound\SoundDevice.h"
#include "core-Sound\SoundChannel.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

Sound3D::Sound3D(const std::string& name, Sound& sound, float soundHearingRadius)
      : Node(name), 
      m_soundHearingRadius(soundHearingRadius),
      m_channel(NULL),
      m_sound(sound),
      m_soundHearingRadiusSq(soundHearingRadius * soundHearingRadius)
{
}

///////////////////////////////////////////////////////////////////////////////

Sound3D::~Sound3D()
{
}

///////////////////////////////////////////////////////////////////////////////

void Sound3D::assignChannel(SoundDevice& device)
{
   if (m_channel != NULL)
   {
      throw std::logic_error("The 3d sound already has a channel assigned");
   }

   m_channel = &(device.activateSound(m_sound));
   onChannelAssigned(*m_channel);
   m_channel->setLooped(true); // TODO:
   m_channel->play();
}

///////////////////////////////////////////////////////////////////////////////

void Sound3D::deassignChannel()
{
   if (m_channel == NULL)
   {
      throw std::logic_error("No channel assigned");
   }
   m_channel->removeSound();
   m_channel = NULL;
}

///////////////////////////////////////////////////////////////////////////////
