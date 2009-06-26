#include "core-Sound\Sound3D.h"
#include "core-Sound\Sound.h"
#include "core-Sound\SoundDevice.h"
#include "core-Sound\SoundChannel.h"
#include "core-Sound\SoundListener.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include <stdexcept>
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

Sound3D::Sound3D(const std::string& name, bool dynamic, Sound& sound, float soundHearingRadius)
      : Node(name, dynamic), 
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

void Sound3D::onAccept(NodeVisitor& visitor)
{
   REGISTER_NODE_VISITOR(TNodesVisitor<Sound3D>);
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

void Sound3D::deassignChannel(SoundDevice& device)
{
   if (m_channel == NULL)
   {
      throw std::logic_error("No channel assigned");
   }
   device.deactivateSound(*m_channel);
   m_channel = NULL;
}

///////////////////////////////////////////////////////////////////////////////
