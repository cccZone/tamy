#include "core-ResourceManagement\ParticleSystemFactory.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "impl-DirectX\D3DParticleSystem.h"
#include "impl-DirectX\D3DRenderer.h"
#include <d3d9.h>


//////////////////////////////////////////////////////////////////////////////

ParticleSystem* ParticleSystemFactory<IDirect3D9>::operator()(const std::string& name, 
                                                              bool isDynamic, 
                                                              Material& material,
                                                              unsigned int particlesCount)
{
   return new D3DParticleSystem(m_resMgr.shared<IDirect3DDevice9>(),
                                name, isDynamic, material, particlesCount);
}

//////////////////////////////////////////////////////////////////////////////
