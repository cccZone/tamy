#include "core-ResourceManagement\IWFLoader.h"
#include "core-ResourceManagement\libIWF\iwfFile.h"
#include "core-ResourceManagement\libIWF\iwfObjects.h"
#include "core-ResourceManagement\IWFScene.h"
#include "core-ResourceManagement\IWFMeshLoader.h"
#include <map>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

namespace
{
    const UCHAR authorID[5] = { 'G', 'I', 'L', 'E', 'S' };
};

///////////////////////////////////////////////////////////////////////////////

IWFLoader::IWFLoader(IWFScene& scene)
      : m_scene(scene)
{
}

///////////////////////////////////////////////////////////////////////////////

IWFLoader::~IWFLoader()
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

   // parse internal meshes
   for (UINT i = 0; i < sceneFile.m_vpMeshList.size(); ++i)
   {
      std::string meshName = getUniqueNameForMesh(sceneFile.m_vpMeshList[i]->Name);

      IWFMeshLoader meshLoader(sceneFile.m_vpMeshList[i], 
                               sceneFile.m_vpTextureList, 
                               sceneFile.m_vpMaterialList);

      MeshDefinition mesh; 
      AnimationDefinition animation;
      std::vector<MaterialDefinition> materials;
      meshLoader.parseMesh(mesh, animation, materials, meshName);

      D3DXMATRIX objMtx = reinterpret_cast<D3DXMATRIX&> (sceneFile.m_vpMeshList[i]->ObjectMatrix);
      m_scene.addStaticGeometry(mesh, materials, objMtx);
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

      IWFLightType lightType;
      switch(lightEntity.LightType)
      {
         case LIGHTTYPE_POINT:         lightType = LT_POINT;        break;
         case LIGHTTYPE_SPOT:          lightType = LT_SPOT;         break;
         case LIGHTTYPE_DIRECTIONAL:   lightType = LT_DIRECTIONAL;  break;
         default:                      lightType = LT_POINT;        break;
      }

      m_scene.addLight(fileEntity->Name,
                       ambient, diffuse, specular,
                       lightType,
                       lightEntity.Range,
                       lightEntity.Attenuation0, lightEntity.Attenuation1, lightEntity.Attenuation2,
                       lightEntity.FallOff, lightEntity.Theta, lightEntity.Phi,
                       reinterpret_cast<const D3DXMATRIX&> (fileEntity->ObjectMatrix));                
   }

   // sky boxes
   else if ((fileEntity->EntityAuthorMatches(5, authorID) == true) &&
       (fileEntity->EntityTypeID == CUSTOM_ENTITY_SKYBOX))
   {
      std::vector<std::string> textures = extractSkyBoxTextures(fileEntity->DataArea);
      m_scene.addSkyBox(textures[0], textures[1], 
                        textures[2], textures[3], 
                        textures[4], textures[5]);

   }

   // references
   else if ((fileEntity->EntityAuthorMatches(5, authorID) == true) &&
            (fileEntity->EntityTypeID == CUSTOM_ENTITY_REFERENCE))
   {
      ReferenceEntity reference = extractReference(fileEntity->DataArea);
      if (reference.referenceType == EXTERNAL_REFERENCE)
      {
         std::string meshName = reference.referenceName;
         D3DXMATRIX objMtx = reinterpret_cast<D3DXMATRIX&> (fileEntity->ObjectMatrix);
         m_scene.addDynamicMesh(meshName, objMtx);
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
