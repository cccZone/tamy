#include "ml-IWF\IWFScene.h"
#include "ml-IWF\libIWF\iwfFile.h"
#include "ml-IWF\libIWF\iwfObjects.h"
#include "ml-IWF\IWFMeshLoader.h"
#include "ml-IWF\MeshDefinition.h"
#include "core-Renderer.h"
#include "core\SingletonsManager.h"
#include "core\ResourcesManager.h"
#include <map>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   const UCHAR authorID[5] = { 'G', 'I', 'L', 'E', 'S' };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

IWFScene::IWFScene(Filesystem& fs, 
                   const std::string& fileName)
: m_progressObserver(NULL)
, m_model(NULL)
, m_renderer(NULL)
, m_rm(NULL)
, m_fs(fs)
{
   std::size_t lastPos = fileName.find_last_of( "/\\" );
   if ( lastPos != std::string::npos )
   {
      m_sceneDir = fileName.substr( 0, lastPos );
      m_fileName = fileName.substr( lastPos + 1 );
   }
   else
   {
      m_fileName = fileName;
   }
}

///////////////////////////////////////////////////////////////////////////////

IWFScene::~IWFScene()
{
   m_progressObserver = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void IWFScene::onLoaded(ResourcesManager& mgr)
{
   m_renderer = &(mgr.getInitializers().shared<Renderer> ());
   m_rm = &mgr;
}

///////////////////////////////////////////////////////////////////////////////

void IWFScene::load(Model& scene, IWFScene::ProgressObserver& observer)
{
   m_model = &scene;
   m_progressObserver = &observer;
   loadScene();
}

///////////////////////////////////////////////////////////////////////////////

void IWFScene::loadScene()
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
      m_progressObserver->setProgress((float)entitiesProcessedCount / (float)allEntitiesCount);
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
      addStaticGeometry(meshes, objMtx);

      // update the progress status
      m_progressObserver->setProgress((float)entitiesProcessedCount / (float)allEntitiesCount);
   }

   // cleanup
   sceneFile.ClearObjects();
}

///////////////////////////////////////////////////////////////////////////////

void IWFScene::processEntities(iwfEntity* fileEntity)
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

      addLight(fileEntity->Name,
         ambient, diffuse, specular,
         lightEntity.LightType,
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
      addSkyBox(textures[0], textures[1], 
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
         addDynamicMesh(meshName, objMtx);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

std::string IWFScene::getUniqueNameForMesh(const char* originalMeshName) const
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

std::vector<std::string> IWFScene::extractSkyBoxTextures(UCHAR* skyBoxData) const
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

ReferenceEntity IWFScene::extractReference(UCHAR* referenceData) const
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

void IWFScene::addLight(const std::string& name,
                        const Color& ambient, 
                        const Color& diffuse, 
                        const Color& specular,
                        long lightType,
                        float range,
                        float constAtt, float linearAtt, float quadAtt,
                        float fallOff, float theta, float phi,
                        const D3DXMATRIX& situation)
{
   switch(lightType)
   {
   case LIGHTTYPE_POINT:         break;
   case LIGHTTYPE_SPOT:          break;
   case LIGHTTYPE_DIRECTIONAL:   break;
   default:                      break;
   }
}

///////////////////////////////////////////////////////////////////////////////

void IWFScene::addSkyBox(const std::string& frontTex,
                         const std::string& backTex,
                         const std::string& leftTex,
                         const std::string& rightTex,
                         const std::string& topTex,
                         const std::string& bottomTex)
{
}

///////////////////////////////////////////////////////////////////////////////

void IWFScene::addDynamicMesh(const std::string& meshFileName, 
                              const D3DXMATRIX& situation)
{
}

///////////////////////////////////////////////////////////////////////////////

void IWFScene::addStaticGeometry(std::vector<MeshDefinition> meshes,
                                 const D3DXMATRIX& situation)
{
   EffectResource& effectRes = m_rm->create<EffectResource> ("Renderer/Shaders/SingleTextureEffect.fx");

   // create main mesh
   RenderableJoint* root = new RenderableJoint();
   root->setLocalMtx(situation);

   unsigned int count = meshes.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      MeshDefinition& currMesh = meshes[i];

      TriangleMesh* geometry = new TriangleMesh(currMesh.vertices, currMesh.faces);
      char geomName[128];
      static int loadedMeshesUniqueID = 0;
      sprintf_s(geomName, 128, "%s/%s_geom_%d.tgt", m_sceneDir.c_str(), currMesh.name.c_str(), ++loadedMeshesUniqueID);
      m_rm->addResource(geometry, geomName);

      Renderable* renderable = new Renderable();
      renderable->add(geometry->load());
      renderable->setLocalMtx(currMesh.localMtx);

      SingleTextureEffect* effect = dynamic_cast<SingleTextureEffect*> (effectRes.load());
   
      MaterialDefinition& mat = currMesh.material;
      effect->setMaterial(Material(mat.ambient, mat.diffuse, mat.specular, mat.emissive, mat.power));
      
      if (mat.texName.length() > 0)
      {
         Texture& texture = m_rm->create<Texture> ( m_sceneDir + std::string( "/" ) + mat.texName );
         effect->setTexture(texture);
      }

      renderable->add(effect);

      root->add(renderable);
   }

   // add the mesh to the scene
   m_model->add(root);
}

///////////////////////////////////////////////////////////////////////////////
