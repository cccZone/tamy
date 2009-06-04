#pragma once

#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

class Sound;
class SoundChannel;
class SoundDevice;
class SoundListener;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class is an instance of a sound in the 3D scene
 */
class Sound3D : public Node
{
protected:
   float m_soundHearingRadius;
   SoundChannel* m_channel;

private:
   Sound& m_sound;
   float m_soundHearingRadiusSq;

public:
   Sound3D(const std::string& name, bool dynamic, Sound& sound, float soundHearingRadius);
   virtual ~Sound3D();

   virtual void update(SoundListener& listener) = 0;

   Sound& getSound() {return m_sound;}

   void assignChannel(SoundDevice& device);
   void deassignChannel();
   bool hasChannelAssigned() const {return m_channel != NULL;}

   float getHearingRadius() const {return m_soundHearingRadius;}
   float getHearingRadiusSq() const {return m_soundHearingRadiusSq;}

protected:
   void onAccept(NodeVisitor& visitor);

   virtual void onChannelAssigned(SoundChannel& channel) = 0;
};

///////////////////////////////////////////////////////////////////////////////
