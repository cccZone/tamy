#include "IWFLoader.h"
#include "libIWF\iwfFile.h"
#include "libIWF\iwfObjects.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "GraphicalNode.h"
#include <map>
#include <sstream>
#include "IWFMeshLoader.h"
#include "Material.h"
#include "Light.h"
#include "SkyBox.h"
#include "Renderer.h"
#include "GraphicalEntity.h"



///////////////////////////////////////////////////////////////////////////////

namespace
{
    const UCHAR authorID[5] = { 'G', 'I', 'L', 'E', 'S' };
};

///////////////////////////////////////////////////////////////////////////////

IWFLoader::IWFLoader(ResourceManager& resourceManager, SceneManager& sceneManager)
      : m_resourceManager(resourceManager),
      m_sceneManager(sceneManager)
{
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
      GraphicalEntity* entity = NULL;
      if (m_resourceManager.isGraphicalEntityRegistered(meshName) == false)
      {
         IWFMeshLoader loader(sceneFile.m_vpMeshList[i], 
                              sceneFile.m_vpTextureList, 
                              sceneFile.m_vpMaterialList);

         entity = &m_resourceManager.loadGraphicalEntity(meshName, loader);
      }
      else
      {
         entity = &m_resourceManager.getGraphicalEntity(meshName);
      }

      Node* entityInstance = entity->instantiate(meshName);
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

      Light* light = m_resourceManager.createLight(fileEntity->Name);

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
      std::vector<unsigned int> materials;
      materials.push_back(m_resourceManager.addMaterial(textures.at(0), Color(1, 1, 1, 1), Color(), Color(), Color(), 1));
      materials.push_back(m_resourceManager.addMaterial(textures.at(1), Color(1, 1, 1, 1), Color(), Color(), Color(), 1));
      materials.push_back(m_resourceManager.addMaterial(textures.at(2), Color(1, 1, 1, 1), Color(), Color(), Color(), 1));
      materials.push_back(m_resourceManager.addMaterial(textures.at(3), Color(1, 1, 1, 1), Color(), Color(), Color(), 1));
      materials.push_back(m_resourceManager.addMaterial(textures.at(4), Color(1, 1, 1, 1), Color(), Color(), Color(), 1));
      materials.push_back(m_resourceManager.addMaterial(textures.at(5), Color(1, 1, 1, 1), Color(), Color(), Color(), 1));

      SkyBox& skyBox = m_resourceManager.createSkyBox();
      for (unsigned char i = 0; i < materials.size(); ++i)
      {
         Material& mat = m_resourceManager.getMaterial(materials.at(i));
         skyBox.setMaterial(static_cast<SkyBoxSides> (i), mat);
      }
      m_resourceManager.getRendererInstance().setSkyBox(skyBox);
   }

   // references
   else if ((fileEntity->EntityAuthorMatches(5, authorID) == true) &&
            (fileEntity->EntityTypeID == CUSTOM_ENTITY_REFERENCE))
   {
      ReferenceEntity reference = extractReference(fileEntity->DataArea);
      if (reference.referenceType == EXTERNAL_REFERENCE)
      {
         std::string meshName = reference.referenceName;
         GraphicalEntity* entity = NULL;
         if (m_resourceManager.isGraphicalEntityRegistered(meshName) == false)
         {
            GraphicalEntityLoader& loader = m_resourceManager.getLoaderForFile(meshName);
            entity = &m_resourceManager.loadGraphicalEntity(meshName, loader);
         }
         else
         {
            entity = &m_resourceManager.getGraphicalEntity(meshName);
         }

         static int refCount = 0;
         std::stringstream refName;
         refName << "reference_" << refCount++;
         Node* entityInstance = entity->instantiate(refName.str());
         entityInstance->setLocalMtx(reinterpret_cast<D3DXMATRIX&> (fileEntity->ObjectMatrix));

         m_sceneManager.addNode(entityInstance);
      }
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
