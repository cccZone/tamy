#include "core-ResourceManagement\SoundListenerFactory.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "impl-DirectX\OALSoundListener.h"
#include "impl-DirectX\OpenALSoundSystem.h"


//////////////////////////////////////////////////////////////////////////////

SoundListener* SoundListenerFactory<IDirect3D9>::operator()()
{
   return new OALSoundListener(m_resMgr.shared<OpenALSoundSystem>());
}

//////////////////////////////////////////////////////////////////////////////
