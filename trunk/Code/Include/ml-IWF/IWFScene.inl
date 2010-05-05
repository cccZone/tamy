#ifndef _IWF_SCENE_H
#error "This file can only be included from IWFScene.h"
#else

#include "ml-IWF\libIWF\iwfFile.h"
#include "ml-IWF\libIWF\iwfObjects.h"
#include "ml-IWF\IWFMeshLoader.h"
#include "ml-IWF\MeshDefinition.h"


///////////////////////////////////////////////////////////////////////////////

template< typename TProgressObserver >
void IWFScene::load( Model& scene, ResourcesManager& rm, TProgressObserver& observer )
{
   CFileIWF sceneFile;
   sceneFile.Load(m_fs, m_sceneDir + std::string("/") + m_fileName);

   ULONG allEntitiesCount = sceneFile.m_vpEntityList.size() + sceneFile.m_vpMeshList.size();
   ULONG entitiesProcessedCount = 0;

   // parse entities
   for (ULONG i = 0; i < sceneFile.m_vpEntityList.size(); ++i, ++entitiesProcessedCount)
   {
      processEntities(sceneFile.m_vpEntityList[i]);

      // update the progress status
      observer.setProgress((float)entitiesProcessedCount / (float)allEntitiesCount);
   }

   // parse internal meshes
   for (UINT i = 0; i < sceneFile.m_vpMeshList.size(); ++i, ++entitiesProcessedCount)
   {
      std::string meshName = getUniqueNameForMesh(sceneFile.m_vpMeshList[i]->Name);

      IWFMeshLoader meshLoader(sceneFile.m_vpMeshList[i], 
         sceneFile.m_vpTextureList, 
         sceneFile.m_vpMaterialList);

      std::vector<MeshDefinition> meshes;
      meshLoader.parseMesh(meshes, meshName);

      D3DXMATRIX objMtx = reinterpret_cast<D3DXMATRIX&> (sceneFile.m_vpMeshList[i]->ObjectMatrix);
      addStaticGeometry( scene, rm, meshes, objMtx );

      // update the progress status
      observer.setProgress((float)entitiesProcessedCount / (float)allEntitiesCount);
   }

   // cleanup
   sceneFile.ClearObjects();
}

///////////////////////////////////////////////////////////////////////////////

#endif // _IWF_SCENE_H
