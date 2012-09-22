#include "ml-IWF\IWFScene.h"
#include "ml-IWF\libIWF\iwfFile.h"
#include "ml-IWF\libIWF\iwfObjects.h"
#include "ml-IWF\IWFMeshLoader.h"
#include "ml-IWF\MeshDefinition.h"
#include "core-Renderer.h"
#include "core.h"
#include <map>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   const UCHAR authorID[5] = { 'G', 'I', 'L', 'E', 'S' };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

IWFScene::IWFScene( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer )
   : TResourceImporter< Model >( path, rm, observer )
{
}

///////////////////////////////////////////////////////////////////////////////

IWFScene::~IWFScene()
{
}

///////////////////////////////////////////////////////////////////////////////

void IWFScene::import( Model& scene )
{
   const Filesystem& fs = m_rm.getFilesystem();

   // load the IWF file
   CFileIWF sceneFile;
   sceneFile.Load( fs, m_loadedFileName );

   // parse entities
   m_observer->initialize( "Importing entities", sceneFile.m_vpEntityList.size() );
   for ( ULONG i = 0; i < sceneFile.m_vpEntityList.size(); ++i )
   {
      processEntities(sceneFile.m_vpEntityList[i]);
      m_observer->advance();
   }

   // parse internal meshes
   m_observer->initialize( "Importing meshes", sceneFile.m_vpMeshList.size() );
   for ( UINT i = 0; i < sceneFile.m_vpMeshList.size(); ++i )
   {
      std::string meshName = getUniqueNameForMesh( sceneFile.m_vpMeshList[i]->Name );

      IWFMeshLoader meshLoader( sceneFile.m_vpMeshList[i], sceneFile.m_vpTextureList, sceneFile.m_vpMaterialList );

      std::vector<MeshDefinition> meshes;
      meshLoader.parseMesh(meshes, meshName);

      Matrix objMtx = reinterpret_cast< Matrix& >( sceneFile.m_vpMeshList[i]->ObjectMatrix );
      addStaticGeometry( scene, m_rm, meshes, objMtx, meshName );

      m_observer->advance();
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
         reinterpret_cast<const Matrix&> (fileEntity->ObjectMatrix));                
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
         Matrix objMtx = reinterpret_cast<Matrix&> (fileEntity->ObjectMatrix);
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
                        const Matrix& situation)
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
                              const Matrix& situation)
{
}

///////////////////////////////////////////////////////////////////////////////

void IWFScene::addStaticGeometry( Model& scene, ResourcesManager& rm, std::vector<MeshDefinition> meshes, const Matrix& situation, const std::string& objName )
{
   // create main mesh
   unsigned int count = meshes.size();
   SpatialEntity* root = NULL;
   if ( count > 1 )
   {
      root = new SpatialEntity( objName );
      root->setLocalMtx(situation);
   }

   FilePath sceneDir;
   m_loadedFileName.extractDir( sceneDir );

   for (unsigned int i = 0; i < count; ++i)
   {
      MeshDefinition& currMesh = meshes[i];

      // create the geometry
      TriangleMesh* geometryRes = NULL;
      {
         char geomName[128];
         sprintf_s( geomName, 128, "%s/%s.%s", sceneDir.c_str(), currMesh.name.c_str(), TriangleMesh::getExtension() );

         FilePath geomResPath( geomName );
         if ( ( geometryRes = rm.findResource< TriangleMesh >( geomResPath ) ) == NULL )
         {
            geometryRes = new TriangleMesh( geomResPath, currMesh.vertices, currMesh.faces );
            geometryRes->calculateTangents();
            rm.addResource( geometryRes );
         }
      }


      // create a rendering effect instance
      MaterialDefinition& mat = currMesh.material;
      MaterialEntity* materialEntity = new MaterialEntity( mat.matName );
      {
         char matInstancePath[128];
         sprintf_s( matInstancePath, 128, "%s/%s.%s", sceneDir.c_str(), mat.matName.c_str(), MaterialInstance::getExtension() );

         MaterialInstance* materialInstance = m_rm.create< MaterialInstance >( FilePath( matInstancePath ) );
         materialEntity->setMaterial( materialInstance );

         Material* materialRenderer = m_rm.create< Material >( FilePath( DEFAULT_MATERIAL_RENDERER ), true );
         materialInstance->setMaterialRenderer( materialRenderer );

         materialInstance->accessSurfaceProperties().set( mat.ambient, mat.diffuse, mat.specular, mat.emissive, mat.power );

         if (mat.texName.length() > 0)
         {
            FilePath texName( sceneDir.getRelativePath() + std::string( "/" ) + mat.texName );
            FilePath texResourceName;
            texName.changeFileExtension( Texture::getExtension(), texResourceName );
            Texture* texture = NULL;
            if ( ( texture = rm.findResource< Texture >( texResourceName ) ) == NULL )
            {
               texture = new Texture( texResourceName );
               texture->setImagePath( texName );
               rm.addResource( texture );
            }
            materialInstance->setTexture( MT_DIFFUSE_1, texture );
         }
      }


      // setup the geometry node
      Geometry* geometry = new StaticGeometry( *geometryRes );
      geometry->setLocalMtx( currMesh.localMtx );
      geometry->add( materialEntity );

      // add the geometry to the mesh
      if ( root )
      {
         root->add( geometry );
      }
      else
      {
         ASSERT_MSG( count == 1, "Only a single mesh can be dealt with this way" );
         root = geometry;
      }
   }

   // add the mesh to the scene
   scene.add( root );
}

///////////////////////////////////////////////////////////////////////////////
