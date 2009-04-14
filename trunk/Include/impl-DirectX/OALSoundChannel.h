#pragma once

#include "core-Sound\SoundChannel.h"
#include <al.h>
#include <alc.h>
#include <list>


///////////////////////////////////////////////////////////////////////////////

class OpenALSoundSystem;

///////////////////////////////////////////////////////////////////////////////

class OALSoundChannel : public SoundChannel
{
private:
   OpenALSoundSystem& m_soundSystem;
   int m_buffersCount;
   ALuint* m_oalBuffer;
   ALuint m_oalSource;

   std::list<ALuint> m_freeBuffers;

public:
   OALSoundChannel(OpenALSoundSystem& soundSystem, int numBuffersUsed);
   ~OALSoundChannel();

   void update();
   int getActiveBuffersCount() const;

protected:
   void addDataToPlayBuffer(char* data, DWORD size,
                            const std::string& format, 
                            DWORD freq);
   void cleanBuffers();
   void startPlaying();
   void stopPlaying();
};

///////////////////////////////////////////////////////////////////////////////
