#include "impl-DirectX\OALSound3D.h"
#include "impl-DirectX\OALSoundChannel.h"
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

OALSound3D::OALSound3D(const std::string& name, 
                       Sound& sound, 
                       float soundHearingRadius)
      : Sound3D(name, sound, soundHearingRadius),
      m_oalSource(0)
{
}

///////////////////////////////////////////////////////////////////////////////

void OALSound3D::update()
{
   if (m_oalSource == 0) {return;}

   D3DXMATRIX mtx = getGlobalMtx();

   ALfloat listenerOri[] = {mtx._31, mtx._32, mtx._33, 
                            mtx._21, mtx._22, mtx._23};

   alSource3f(m_oalSource, AL_POSITION, mtx._41, mtx._42, mtx._42);
   alSourcefv(m_oalSource, AL_ORIENTATION, listenerOri);
}

///////////////////////////////////////////////////////////////////////////////

void OALSound3D::onChannelAssigned(SoundChannel& channel)
{
   OALSoundChannel* oalChannel = dynamic_cast<OALSoundChannel*> (&channel);
   assert(oalChannel != NULL);

   m_oalSource = oalChannel->getOALSource();
   alSourcei(m_oalSource, AL_SOURCE_RELATIVE, AL_FALSE);
   alSourcef(m_oalSource, AL_MAX_DISTANCE, m_soundHearingRadius);
   alSourcef(m_oalSource, AL_ROLLOFF_FACTOR, 2);
   alSourcef(m_oalSource, AL_REFERENCE_DISTANCE, m_soundHearingRadius * 0.1f);
}

///////////////////////////////////////////////////////////////////////////////
