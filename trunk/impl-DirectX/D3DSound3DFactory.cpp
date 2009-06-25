#include "core-ResourceManagement\Sound3DFactory.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "impl-DirectX\OALSound3D.h"
#include "impl-DirectX\OpenALSoundSystem.h"


//////////////////////////////////////////////////////////////////////////////

Sound3D* Sound3DFactory<IDirect3D9>::operator()(const std::string& name, 
                                                bool dynamic,
                                                Sound& sound, 
                                                float hearingRadius)
{
   return new OALSound3D(m_resMgr.shared<OpenALSoundSystem>(), 
                         name, dynamic, sound, hearingRadius);
}

//////////////////////////////////////////////////////////////////////////////
