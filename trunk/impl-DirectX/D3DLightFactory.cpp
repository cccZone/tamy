#include "core-ResourceManagement\LightFactory.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "impl-DirectX\D3DLight.h"
#include "impl-DirectX\D3DRenderer.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

template<>
Light* LightFactory<IDirect3D9>::operator()(const std::string& name)
{
   return new D3DLight(name, m_resMgr.shared<IDirect3DDevice9>());
}

///////////////////////////////////////////////////////////////////////////////
