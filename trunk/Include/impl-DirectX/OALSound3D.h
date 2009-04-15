#pragma once

#include "core-Sound\Sound3D.h"
#include <al.h>
#include <alc.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This is an OpenAL implementation of a 3D scene sound
 */
class OALSound3D : public Sound3D
{
private:
   ALuint m_oalSource;

public:
   OALSound3D(const std::string& name, Sound& sound, float soundHearingRadius);

   void update();

protected:
   void onChannelAssigned(SoundChannel& channel);
};

///////////////////////////////////////////////////////////////////////////////
