#include "core-ResourceManagement\GraphicalEntityFactory.h"
#include "impl-DirectX\D3DGraphicalEntity.h"
#include "impl-DirectX\D3DSkinnedGraphicalEntity.h"
#include "impl-DirectX\D3DRenderer.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "core-ResourceManagement\LitVertex.h"
#include <d3d9.h>


//////////////////////////////////////////////////////////////////////////////

GraphicalEntity* GraphicalEntityFactory<IDirect3D9>::createGraphicalEntity(
                                       const std::string& name,
                                       const MeshDefinition& subMesh,
                                       const std::vector<Material*>& registeredMaterials)
{
   return new D3DGraphicalEntity<LitVertex>(name, 
                                            m_resMgr.shared<IDirect3DDevice9>(), 
                                            subMesh,
                                            registeredMaterials);
}

//////////////////////////////////////////////////////////////////////////////

SkinnedGraphicalEntity* GraphicalEntityFactory<IDirect3D9>::createSkinedGraphicalEntity(
                                                    const std::string& name,
                                                    const MeshDefinition& subMesh,
                                                    const std::vector<Material*>& registeredMaterials)
{
   return new D3DSkinnedGraphicalEntity<LitVertex>(name, 
                                                   m_resMgr.shared<IDirect3DDevice9>(), 
                                                   subMesh,
                                                   registeredMaterials);
}

//////////////////////////////////////////////////////////////////////////////
