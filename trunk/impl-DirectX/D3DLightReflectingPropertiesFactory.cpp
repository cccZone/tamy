#include "core-ResourceManagement\LightReflectingPropertiesFactory.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "impl-DirectX\D3DLightReflectingProperties.h"
#include "impl-DirectX\D3DRenderer.h"
#include <d3d9.h>


//////////////////////////////////////////////////////////////////////////////

LightReflectingProperties* 
LightReflectingPropertiesFactory<IDirect3D9>::operator()()
{
   return new D3DLightReflectingProperties(m_resMgr.shared<IDirect3DDevice9>());
}

//////////////////////////////////////////////////////////////////////////////
