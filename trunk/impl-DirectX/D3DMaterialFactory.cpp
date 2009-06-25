#include "core-ResourceManagement\MaterialFactory.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "impl-DirectX\D3DMaterial.h"
#include "impl-DirectX\D3DRenderer.h"
#include "impl-DirectX\D3DAlphaOperationImplementation.h"
#include "impl-DirectX\D3DColorOperationImplementation.h"
#include <d3d9.h>


//////////////////////////////////////////////////////////////////////////////

Material* MaterialFactory<IDirect3D9>::create(const std::string& name,
                                              unsigned int index, 
                                              LightReflectingProperties* lrp)
{
   return new D3DMaterial(m_resMgr.shared<IDirect3DDevice9>(), 
                          name,
                          lrp, 
                          m_resMgr.shared<D3DAlphaOperationImplementation>(),
                          m_resMgr.shared<D3DColorOperationImplementation>(),
                          index);
}

//////////////////////////////////////////////////////////////////////////////
