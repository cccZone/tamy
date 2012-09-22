#include "ml-Blender/BlenderSceneExporter.h"
#include "core.h"
#include "core-Renderer.h"
#include "core-MVC.h"

#include "ml-Blender/TamyMaterial.h"
#include "ml-Blender/TamyMesh.h"
#include "ml-Blender/TamyScene.h"


///////////////////////////////////////////////////////////////////////////////

#define TEXTURES_EXPORT_DIR      std::string( "/Textures/" )
#define MATERIALS_EXPORT_DIR     std::string( "/Material/" )
#define MESHES_EXPORT_DIR        std::string( "/Meshes/" )
#define SCENES_EXPORT_DIR        std::string( "/Scenes/" )

///////////////////////////////////////////////////////////////////////////////

BlenderSceneExporter BlenderSceneExporter::s_theIntance;

///////////////////////////////////////////////////////////////////////////////

BlenderSceneExporter::BlenderSceneExporter()
   : m_materialRenderer( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::initialize( const char* filesystemRoot, const char* exportDir, int entitiesCount ) 
{ 
   // set the filesystem path
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();

   std::string normalizedFilesystemRoot;
   FilesystemUtils::normalize( filesystemRoot, normalizedFilesystemRoot );
   fs.changeRootDir( normalizedFilesystemRoot );

   // setup the export directories
   std::string normalizedExportDir;
   FilesystemUtils::normalize( exportDir, normalizedExportDir );
   std::string relativeExportDir = fs.toRelativePath( normalizedExportDir );

   m_texturesExportDir = relativeExportDir + TEXTURES_EXPORT_DIR;
   m_meshesExportDir = relativeExportDir + MESHES_EXPORT_DIR;
   m_scenesExportDir = relativeExportDir + SCENES_EXPORT_DIR;
   m_materialsExportDir = relativeExportDir + MATERIALS_EXPORT_DIR;

   // physically create the directories
   FilesystemUtils::mkdir( fs.toAbsolutePath( m_texturesExportDir ).c_str() ); 
   FilesystemUtils::mkdir( fs.toAbsolutePath( m_meshesExportDir ).c_str() );
   FilesystemUtils::mkdir( fs.toAbsolutePath( m_scenesExportDir ).c_str() );
   FilesystemUtils::mkdir( fs.toAbsolutePath( m_materialsExportDir ).c_str() );

   // load the common material resource that will be used by all exported material entities
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   m_materialRenderer = resMgr.create< Material >( FilePath( DEFAULT_MATERIAL_RENDERER ), true );
   ASSERT_MSG( m_materialRenderer != NULL, "Required material renderer resource not found" );

   // clear the storages
   m_textures.clear();
   m_materialDefinitions.clear();

   m_exportedEntities.clear();
   m_exportedEntities.resize( entitiesCount );
   m_nextEntityIdx = 0;

   m_worldEntities.clear();
}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::reset()
{
   m_materialRenderer = NULL;
   m_textures.clear();

   m_materialDefinitions.clear();

   m_exportedEntities.clear();
   m_worldEntities.clear();
}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::createTexture( const TamyTexture& exportedTexture )
{
   Texture* texture = NULL;
   if ( exportedTexture.path != NULL )
   {
      ResourcesManager& resMgr = ResourcesManager::getInstance();

      // copy the actual image to the filesystem root
      std::string normalizedTexturePath;
      FilesystemUtils::normalize( exportedTexture.path, normalizedTexturePath );
      std::string imgFileName = FilesystemUtils::extractNodeName( normalizedTexturePath );
      FilePath newImagePath( m_texturesExportDir + imgFileName );

      std::string absImagePath = newImagePath.toAbsolutePath( resMgr.getFilesystem() );
      FilesystemUtils::copyFile( exportedTexture.path, absImagePath.c_str() );

      // create and setup a texture resource for it
      texture = resMgr.create< Texture >( newImagePath );
      texture->setImagePath( newImagePath );

      // save the texture
      texture->saveResource();
   }

   // add the texture to the lookups storage - we're adding NULLs as well here,
   // because the indexing order is crucial
   m_textures.push_back( texture );
}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::storeMaterials( TamyMaterial* arrMaterials, int materialsCount )
{
   ASSERT_MSG( m_materialDefinitions.empty(), "This method is going to override the existing materials definitions!" );

   ResourcesManager& resMgr = ResourcesManager::getInstance();

   // delete previous definitions
   m_materialDefinitions.clear();
   m_materialDefinitions.resize( materialsCount );

   char tmpMatPath[128];
   for( int i = 0; i < materialsCount; ++i )
   {
      const TamyMaterial& exportedMaterial = arrMaterials[i];
      m_materialDefinitions[i].m_materialName = exportedMaterial.name;

      sprintf_s( tmpMatPath , "%s%s.%s", m_materialsExportDir.c_str(), exportedMaterial.name, MaterialInstance::getExtension() );
      m_materialDefinitions[i].m_path = FilePath( tmpMatPath );

      MaterialInstance* matInstance = resMgr.create< MaterialInstance >( m_materialDefinitions[i].m_path );
      matInstance->setMaterialRenderer( m_materialRenderer );

      // setup the material properties
      {
         // surface properties setup
         SurfaceProperties& surfaceProperties = matInstance->accessSurfaceProperties();
         surfaceProperties.setAmbientColor( exportedMaterial.ambientColor );
         surfaceProperties.setDiffuseColor( exportedMaterial.diffuseColor );
         surfaceProperties.setSpecularColor( exportedMaterial.specularColor );

         // textures setup
         if ( exportedMaterial.normalTextureIndex >= 0 && exportedMaterial.normalTextureIndex < (int)m_textures.size() )
         {
            Texture* normalTexture = m_textures[ exportedMaterial.normalTextureIndex ];
            matInstance->setTexture( MT_NORMALS, normalTexture );
         }

         if ( exportedMaterial.specularTextureIndex >= 0 && exportedMaterial.specularTextureIndex < (int)m_textures.size() )
         {
            Texture* specularTexture = m_textures[ exportedMaterial.specularTextureIndex ];
            matInstance->setTexture( MT_SPECULAR, specularTexture );
         }

         for ( int i = MT_DIFFUSE_1; i < MT_DIFFUSE_2; ++i )
         {
            int mapIdx = i - MT_DIFFUSE_1;
            if ( mapIdx < exportedMaterial.diffuseTexturesCount )
            {
               int diffuseTextureIdx = exportedMaterial.diffuseTexturesIndices[mapIdx];
               Texture* diffuseTexture = m_textures[ diffuseTextureIdx ];
               matInstance->setTexture( (MaterialTextures)i, diffuseTexture );
            }
         }
      }

      // save the material
      matInstance->saveResource();
   }


}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::addEntity( SpatialEntity* entity, const Matrix& transform, int parentIdx )
{
   ExportedEntity& exportedEntity = m_exportedEntities[m_nextEntityIdx];
   exportedEntity.m_entity = entity;
   exportedEntity.m_parentIndex = parentIdx;
   exportedEntity.m_globalMtx = transform;
   ++m_nextEntityIdx;
}

///////////////////////////////////////////////////////////////////////////////

TriangleMesh* BlenderSceneExporter::createMesh( const TamyMesh& exportedMesh )
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   // create the resource
   FilePath meshPath( m_meshesExportDir + exportedMesh.name + "." + TriangleMesh::getExtension() );
   TriangleMesh* triangleMesh = resMgr.create< TriangleMesh >( meshPath );

   // set mesh data
   triangleMesh->setVertices( exportedMesh.verticesList, exportedMesh.verticesCount );
   triangleMesh->setFaces( exportedMesh.facesList, exportedMesh.facesCount );
   triangleMesh->calculateTangents();

   // save the resource
   triangleMesh->saveResource();

   return triangleMesh;
}

///////////////////////////////////////////////////////////////////////////////

SpatialEntity* BlenderSceneExporter::createGeometryEntity( const TamyGeometry& exportedGeometryEntity ) 
{
   if ( exportedGeometryEntity.meshesCount == 0 )
   {
      // nothing to do here, but we still need to fill the slot in the entities array
      return NULL;
   }

   if ( exportedGeometryEntity.meshesCount == 1 )
   {
      Geometry* geometry = instantiateStaticGeometry( exportedGeometryEntity, 0, true );
      return geometry;
   }
   else
   {
      // there's more than one mesh in the entity, so aggregate them under one SpatialEntity
      SpatialEntity* parentEntity = new SpatialEntity( exportedGeometryEntity.name );

      for ( int i = 0; i < exportedGeometryEntity.meshesCount; ++i )
      {
         Geometry* geometry = instantiateStaticGeometry( exportedGeometryEntity, i, false );
         if ( geometry )
         {
            parentEntity->add( geometry );
         }
      }

      return parentEntity;
   }
}

///////////////////////////////////////////////////////////////////////////////

Geometry* BlenderSceneExporter::instantiateStaticGeometry( const TamyGeometry& exportedGeometryEntity, int meshIdx, bool useOriginalName )
{
   Geometry* geometry = NULL;

   const TamyMesh& exportedMesh = exportedGeometryEntity.meshesList[0];

   // create the mesh
   TriangleMesh* mesh = createMesh( exportedMesh );
   ASSERT_MSG( mesh != NULL, "Specified mesh wasn't correctly loaded" );
   if ( mesh != NULL )
   {
      char entityName[512];
      if ( useOriginalName )
      {
         sprintf_s( entityName, "%s", exportedGeometryEntity.name );
      }
      else
      {
         sprintf_s( entityName, "%s_%d", exportedGeometryEntity.name, meshIdx );
      }

      // at the moment we're importing only the static geometry
      geometry = new StaticGeometry( *mesh, entityName );

      // now attach the defined materials to it
      for ( int i = 0; i < exportedMesh.materialsCount; ++i )
      {
         int materialIdx = exportedMesh.materialsList[i];
         attachMaterialToGeometry( geometry, materialIdx );
      }
   }

   return geometry;
}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::attachMaterialToGeometry( Geometry* geometryEntity, uint materialIdx ) const
{
   ASSERT_MSG( materialIdx < m_materialDefinitions.size(), "Specified material wasn't exported" );
   if ( materialIdx >= m_materialDefinitions.size() )
   {
      return;
   }
   const MaterialDefinition& materialDef = m_materialDefinitions[materialIdx];
   MaterialEntity* materialEntity = new MaterialEntity( materialDef.m_materialName );

   ResourcesManager& resMgr = ResourcesManager::getInstance();
   MaterialInstance* material = resMgr.create< MaterialInstance >( materialDef.m_path, true );
   materialEntity->setMaterial( material );

   geometryEntity->add( materialEntity );
}

///////////////////////////////////////////////////////////////////////////////

SpatialEntity* BlenderSceneExporter::createLightEntity( const TamyLight& exportedLightEntity )
{
   SpatialEntity* lightEntity = NULL;

   switch( exportedLightEntity.type )
   {
   case LT_Directional:
      {
         lightEntity = instantiateDirectionalLight( exportedLightEntity );
         break;
      }

   case LT_Point:
      {
         lightEntity = instantiatePointLight( exportedLightEntity );
         break;
      }

   case LT_Spotlight:
      {
         // not implemented yet
         break;
      }

   case LT_Hemisphere:
      {
         // not implemented yet
         break;
      }

   case LT_Area:
      {
         // not implemented yet
         break;
      }

   }

   return lightEntity;
}

///////////////////////////////////////////////////////////////////////////////

SpatialEntity* BlenderSceneExporter::instantiateDirectionalLight( const TamyLight& exportedLightEntity )
{
   DirectionalLight* light = new DirectionalLight( exportedLightEntity.name );

   light->m_color = exportedLightEntity.lightColor;
   light->m_strength = exportedLightEntity.energy;
   light->setShadowsCaster( exportedLightEntity.castShadows );

   return light;
}

///////////////////////////////////////////////////////////////////////////////

SpatialEntity* BlenderSceneExporter::instantiatePointLight( const TamyLight& exportedLightEntity )
{
   PointLight* light = new PointLight( exportedLightEntity.name );

   light->m_color = exportedLightEntity.lightColor;
   light->m_strength = exportedLightEntity.energy;
   light->m_radius = exportedLightEntity.distance;
   light->m_attenuation = exportedLightEntity.linearAttenuation;
   light->setShadowsCaster( exportedLightEntity.castShadows );

   return light;
}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::setupWorld( const TamyWorld& worldSettings )
{
   // create an ambient light
   {
      AmbientLight* ambientLight = new AmbientLight( "AmbientLight" );
      ambientLight->m_lightColor = worldSettings.ambientLightColor;

      m_worldEntities.push_back( ambientLight );
   }
}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::assembleScene( const char* sceneName )
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   // create the resource
   FilePath scenePath( m_scenesExportDir + sceneName );
   Model* scene = resMgr.create< Model >( scenePath );

   // if the scene existed, delete its contents
   scene->clear();

   // setup the parenting hierarchy
   Matrix localMtx, invParentGlobalMtx;
   uint entitiesCount = m_exportedEntities.size();
   for ( uint i = 0; i < entitiesCount; ++i )
   {
      ExportedEntity& exportedEntity = m_exportedEntities[i];

      if ( exportedEntity.m_parentIndex >= 0 )
      {
         // this one has a parent
         ExportedEntity& exportedParentEntity = m_exportedEntities[ exportedEntity.m_parentIndex ];
         SpatialEntity* parentEntity = exportedParentEntity.m_entity;
         parentEntity->add( exportedEntity.m_entity );

         // set the matrix of it
         invParentGlobalMtx.setInverse( exportedParentEntity.m_globalMtx );
         localMtx.setMul( exportedEntity.m_globalMtx, invParentGlobalMtx );
         exportedEntity.m_entity->setLocalMtx( localMtx );
      }
      else
      {
         // this is the top level entity
         scene->add( exportedEntity.m_entity );
         exportedEntity.m_entity->setLocalMtx( exportedEntity.m_globalMtx );
      }
   }

   // add the world-related entities
   entitiesCount = m_worldEntities.size();
   if ( entitiesCount > 0 )
   {
      // group them under one entity, for the ease of access
      SpatialEntity* worldEntities = new SpatialEntity( "World" );

      for ( uint i = 0; i < entitiesCount; ++i )
      {
         Entity* entity = m_worldEntities[i];
         worldEntities->add( entity );
      }

      scene->add( worldEntities );
   }

   // save the resource
   scene->saveResource();

   // cleanup
   reset();
}

///////////////////////////////////////////////////////////////////////////////
