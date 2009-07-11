#include "core-ResourceManagement\IWFLoader.h"
#include "core-ResourceManagement\libIWF\iwfFile.h"
#include "core-ResourceManagement\libIWF\iwfObjects.h"
#include "core\SceneManager.h"
#include <map>
#include <sstream>
#include "core-ResourceManagement\IWFMeshLoader.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"

#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Texture.h"


///////////////////////////////////////////////////////////////////////////////

namespace
{
    const UCHAR authorID[5] = { 'G', 'I', 'L', 'E', 'S' };
};

///////////////////////////////////////////////////////////////////////////////

IWFLoader::IWFLoader(GraphicalEntitiesFactory& entitiesFactory,
                     GraphicalDataSource& externalEntitiesSource,
                     SceneManager& sceneManager,
                     ResourceStorage<AbstractGraphicalEntity>& entitiesStorage,
                     ResourceStorage<Material>& materialsStorage)
      : m_entitiesFactory(entitiesFactory),
      m_entityLoader(new GraphicalEntityLoader(entitiesFactory, materialsStorage)),
      m_externalEntitiesSource(externalEntitiesSource),
      m_sceneManager(sceneManager),
      m_entitiesStorage(entitiesStorage),
      m_materialsStorage(materialsStorage)
{
}

///////////////////////////////////////////////////////////////////////////////

IWFLoader::~IWFLoader()
{
   delete m_entityLoader;
   m_entityLoader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void IWFLoader::load(const std::string& fileName)
{
   CFileIWF sceneFile;

   LPCSTR strFileName = fileName.c_str();
   sceneFile.Load(strFileName);

   // parse entities
   for (ULONG i = 0; i < sceneFile.m_vpEntityList.size(); ++i)
   {
      processEntities(sceneFile.m_vpEntityList[i]);
   }

   // parse meshes
   for (UINT i = 0; i < sceneFile.m_vpMeshList.size(); ++i)
   {
      std::string meshName = getUniqueNameForMesh(sceneFile.m_vpMeshList[i]->Name);
      AbstractGraphicalEntity* entity = NULL;
      if (m_entitiesStorage.is(meshName))
      {
         entity = &(m_entitiesStorage.get(meshName));
      }
      else
      {
         IWFMeshLoader source(sceneFile.m_vpMeshList[i], 
                              sceneFile.m_vpTextureList, 
                              sceneFile.m_vpMaterialList);
         entity = m_entityLoader->load(meshName, source); 
         m_entitiesStorage.add(entity);
      }

      GraphicalEntityInstantiator* entityInstance = new GraphicalEntityInstantiator("meshName", false);
      entityInstance->attachEntity(*entity);
      entityInstance->setLocalMtx(reinterpret_cast<D3DXMATRIX&> (sceneFile.m_vpMeshList[i]->ObjectMatrix));

      m_sceneManager.addNode(entityInstance);
   }

   // cleanup
   sceneFile.ClearObjects();
}

///////////////////////////////////////////////////////////////////////////////

void IWFLoader::processEntities(iwfEntity* fileEntity)
{
   if (fileEntity->DataSize == 0) return;

   // lights
   if (fileEntity->EntityTypeMatches(ENTITY_LIGHT) == true) 
   {
      LIGHTENTITY& lightEntity = *(reinterpret_cast<LIGHTENTITY*> (fileEntity->DataArea));
      Color ambient;
      ambient.r = lightEntity.AmbientRed;
      ambient.g = lightEntity.AmbientGreen;
      ambient.b = lightEntity.AmbientBlue;
      ambient.a = lightEntity.AmbientAlpha;

      Color diffuse;
      diffuse.r = lightEntity.DiffuseRed;
      diffuse.g = lightEntity.DiffuseGreen;
      diffuse.b = lightEntity.DiffuseBlue;
      diffuse.a = lightEntity.DiffuseAlpha;

      Color specular;
      specular.r = lightEntity.SpecularRed;
      specular.g = lightEntity.SpecularGreen;
      specular.b = lightEntity.SpecularBlue;
      specular.a = lightEntity.SpecularAlpha;

      Light* light = m_entitiesFactory.createLight(fileEntity->Name);

      light->setAmbientColor(ambient);
      light->setDiffuseColor(diffuse);
      light->setSpecularColor(specular);

      switch(lightEntity.LightType)
      {
         case LIGHTTYPE_POINT:         light->setType(Light::LT_POINT);        break;
         case LIGHTTYPE_SPOT:          light->setType(Light::LT_SPOT);         break;
         case LIGHTTYPE_DIRECTIONAL:   light->setType(Light::LT_DIRECTIONAL);  break;
         default:                                                             break;
      }
      light->setRange(lightEntity.Range);
      light->setAttenuation(lightEntity.Attenuation0, lightEntity.Attenuation1, lightEntity.Attenuation2);
      light->setSpotlightFeatures(lightEntity.FallOff, lightEntity.Theta, lightEntity.Phi);
      light->setLocalMtx(reinterpret_cast<const D3DXMATRIX&> (fileEntity->ObjectMatrix));

      m_sceneManager.addNode(light);
   }

   // sky boxes
   else if ((fileEntity->EntityAuthorMatches(5, authorID) == true) &&
       (fileEntity->EntityTypeID == CUSTOM_ENTITY_SKYBOX))
   {
      std::vector<std::string> textures = extractSkyBoxTextures(fileEntity->DataArea);

      SkyBox* skyBox = m_entitiesFactory.createSkyBox(
                           createSkyboxMaterial(textures.at(0)),
                           createSkyboxMaterial(textures.at(1)),
                           createSkyboxMaterial(textures.at(2)),
                           createSkyboxMaterial(textures.at(3)),
                           createSkyboxMaterial(textures.at(4)),
                           createSkyboxMaterial(textures.at(5)));

      m_sceneManager.addNode(skyBox);
   }

   // references
   else if ((fileEntity->EntityAuthorMatches(5, authorID) == true) &&
            (fileEntity->EntityTypeID == CUSTOM_ENTITY_REFERENCE))
   {
      ReferenceEntity reference = extractReference(fileEntity->DataArea);
      if (reference.referenceType == EXTERNAL_REFERENCE)
      {
         std::string meshName = reference.referenceName;

         AbstractGraphicalEntity* entity = NULL;
         if (m_entitiesStorage.is(meshName))
         {
            entity = &(m_entitiesStorage.get(meshName));
         }
         else
         {
            entity = m_entityLoader->load(meshName, m_externalEntitiesSource);
            m_entitiesStorage.add(entity);
         }
         
         static int refCount = 0;
         std::stringstream refName;
         refName << "reference_" << refCount++;
         GraphicalEntityInstantiator* entityInstance = new GraphicalEntityInstantiator(refName.str(), false);
         entityInstance->attachEntity(*entity);
         entityInstance->setLocalMtx(reinterpret_cast<D3DXMATRIX&> (fileEntity->ObjectMatrix));

         m_sceneManager.addNode(entityInstance);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

Material& IWFLoader::createSkyboxMaterial(const std::string& textureName) const
{
   std::string materialName = std::string("skyBox_") + textureName;

   if (m_materialsStorage.is(materialName))
   {
      return m_materialsStorage.get(materialName);
   }
   else
   {
      LightReflectingProperties* lrp = m_entitiesFactory.createLightReflectingProperties();
      lrp->setAmbientColor(Color(1, 1, 1, 1));
      lrp->setDiffuseColor(Color());
      lrp->setSpecularColor(Color());
      lrp->setEmissiveColor(Color());
      lrp->setPower(1);
         
      MaterialStage* stage = m_entitiesFactory.createMaterialStage(textureName,
                                                                   MOP_SELECT_ARG1, SC_TEXTURE, SC_NONE,
                                                                   MOP_DISABLE, SC_NONE, SC_NONE,
                                                                   CC_CLAMP);

      Material* mat = m_entitiesFactory.createMaterial(materialName, lrp);
      mat->addStage(stage);
      m_materialsStorage.add(mat);
      return *mat;
   }
}

///////////////////////////////////////////////////////////////////////////////

std::string IWFLoader::getUniqueNameForMesh(const char* originalMeshName) const
{
   char name[1024];
   static int uniqueInt = 0;
   if (originalMeshName == NULL)
   {
      SYSTEMTIME st;
      GetSystemTime (&st);
      sprintf_s(name, 1024, "internalMesh_%d%d%d%d%d%d%d_%d", 
                st.wMilliseconds, st.wSecond, st.wMinute, 
                st.wHour, st.wDay, st.wMonth, st.wYear, uniqueInt++);
   }
   else
   {
      sprintf_s(name, 1024, "%s", originalMeshName);
   }

   return name;
}

///////////////////////////////////////////////////////////////////////////////

std::vector<std::string> IWFLoader::extractSkyBoxTextures(UCHAR* skyBoxData) const
{
   // the first entry for the record are the flags, but we're not interested in them
   skyBoxData += sizeof(ULONG);

   std::vector<std::string> textures;
   // read each of the 6 face textures
   for (UINT j = 0; j < 6; ++j)
   {
      USHORT stringLength;
      memcpy(&stringLength, skyBoxData, sizeof(USHORT));
      skyBoxData += sizeof(USHORT);

      if (stringLength > 0) 
      {
         char tmpText[1024];
         ZeroMemory(tmpText, 1024);
         if (stringLength > 1023) {stringLength = 1023;}
         strncpy_s(tmpText, reinterpret_cast<char*> (skyBoxData), stringLength);
         textures.push_back(tmpText);
      }
      skyBoxData += stringLength;
   }  

   return textures;
}

///////////////////////////////////////////////////////////////////////////////

ReferenceEntity IWFLoader::extractReference(UCHAR* referenceData) const
{
   ReferenceEntity reference;

   memcpy(&reference.referenceType, referenceData, sizeof(ULONG));
   referenceData += sizeof(ULONG);

   memcpy(&reference.flags, referenceData, sizeof(ULONG));
   referenceData += sizeof(ULONG);

   USHORT stringLength;
   memcpy(&stringLength, referenceData, sizeof(USHORT));
   referenceData += sizeof(USHORT);
   if (stringLength > 0)
   {
      char tmpStr[1024];
      ZeroMemory(tmpStr, 1024);
      strncpy_s(tmpStr, reinterpret_cast<char*> (referenceData), stringLength);
      reference.referenceName = tmpStr;
   }

   return reference;
}

///////////////////////////////////////////////////////////////////////////////
