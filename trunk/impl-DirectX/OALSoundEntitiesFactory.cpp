#include "impl-DirectX\OALSoundEntitiesFactory.h"
#include "impl-DirectX\OpenALSoundSystem.h"
#include "impl-DirectX\OALSound3D.h"
#include "impl-DirectX\OALSoundListener.h"


///////////////////////////////////////////////////////////////////////////////

OALSoundEntitiesFactory::OALSoundEntitiesFactory(OpenALSoundSystem& soundSystem)
      : m_soundSystem(soundSystem)
{
}

///////////////////////////////////////////////////////////////////////////////

Sound3D* OALSoundEntitiesFactory::createEmiter(const std::string& name, 
                                               bool dynamic,
                                               Sound& sound, 
                                               float hearingRadius)
{
   return new OALSound3D(m_soundSystem, name, dynamic, sound, hearingRadius);
}

///////////////////////////////////////////////////////////////////////////////

SoundListener* OALSoundEntitiesFactory::createListener()
{
   return new OALSoundListener(m_soundSystem);
}

///////////////////////////////////////////////////////////////////////////////