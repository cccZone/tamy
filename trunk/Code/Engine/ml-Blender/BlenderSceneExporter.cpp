#include "ml-Blender/BlenderSceneExporter.h"
#include "core.h"
#include "core-Renderer.h"
#include "core-MVC.h"

#include "ml-Blender/TamyMaterial.h"
#include "ml-Blender/TamyMesh.h"
#include "ml-Blender/TamyScene.h"


///////////////////////////////////////////////////////////////////////////////

#define TEXTURES_EXPORT_DIR      std::string( "/Textures/" )
#define MATERIAL_PATH            std::string( "/Renderer/Materials/simpleTextured.tmat" )
#define MESHES_EXPORT_DIR        std::string( "/Meshes/" )
#define SCENES_EXPORT_DIR        std::string( "/Scenes/" )

///////////////////////////////////////////////////////////////////////////////

BlenderSceneExporter BlenderSceneExporter::s_theIntance;

///////////////////////////////////////////////////////////////////////////////

BlenderSceneExporter::BlenderSceneExporter()
   : m_material( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::initialize( const char* filesystemRoot, const char* exportDir ) 
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

   // physically create the directories
   FilesystemUtils::mkdir( fs.toAbsolutePath( m_texturesExportDir ).c_str() ); 
   FilesystemUtils::mkdir( fs.toAbsolutePath( m_meshesExportDir ).c_str() );
   FilesystemUtils::mkdir( fs.toAbsolutePath( m_scenesExportDir ).c_str() );

   // load the common material resource that will be used by all exported material entities
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   m_material = resMgr.create< Material >( MATERIAL_PATH, true );
   ASSERT_MSG( m_material != NULL, "Required material resource not found" );

   // clear the storages
   m_textures.clear();

   int count = m_materialDefinitions.size();
   for ( int i = 0; i < count; ++i )
   {
      delete m_materialDefinitions[i];
   }
   m_materialDefinitions.clear();
}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::reset()
{
   m_material = NULL;
   m_textures.clear();

   int count = m_materialDefinitions.size();
   for ( int i = 0; i < count; ++i )
   {
      delete m_materialDefinitions[i];
   }
   m_materialDefinitions.clear();
}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::createTexture( const TamyTexture& exportedTexture )
{
   Texture* texture = NULL;
   if ( exportedTexture.path == NULL )
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

   // delete previous definitions
   int count = m_materialDefinitions.size();
   for ( int i = 0; i < count; ++i )
   {
      delete m_materialDefinitions[i];
   }
   m_materialDefinitions.clear();
   m_materialDefinitions.resize( materialsCount );

   for( int i = 0; i < materialsCount; ++i )
   {
      m_materialDefinitions[i] = new MaterialDefinition( arrMaterials[i] );
   }
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

   // save the resource
   triangleMesh->saveResource();

   return triangleMesh;
}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::createScene( const TamyScene& exportedScene )
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   // create the resource
   FilePath scenePath( m_scenesExportDir + exportedScene.name );
   Model* scene = resMgr.create< Model >( scenePath );

   for ( int i = 0; i < exportedScene.entitiesCount; ++i )
   {
      SpatialEntity* entity = createEntity( exportedScene.entities[i] );
      scene->add( entity );
   }

   // save the resource
   scene->saveResource();
}

///////////////////////////////////////////////////////////////////////////////

SpatialEntity* BlenderSceneExporter::createEntity( const TamyEntity& exportedEntity )
{
   SpatialEntity* entity = new SpatialEntity( exportedEntity.name );
   entity->setLocalMtx( exportedEntity.localMatrix );

   // attach meshes and materials
   for ( int i = 0; i < exportedEntity.meshesCount; ++i )
   {
      const TamyMesh& exportedMesh = exportedEntity.meshesList[i];

      // create the mesh
      TriangleMesh* mesh = createMesh( exportedMesh );
      ASSERT_MSG( mesh != NULL, "Specified mesh wasn't correctly loaded" );
      if ( mesh == NULL )
      {
         continue;
      }

      // at the moment we're importing only static entities
      Geometry* geometry = new StaticGeometry( *mesh );
      entity->add( geometry );

      // now attach the defined materials to it
      for ( int i = 0; i < exportedMesh.materialsCount; ++i )
      {
         int materialIdx = exportedMesh.materialsList[i];
         attachMaterialToGeometry( geometry, materialIdx );
      }
   }

   return entity;
}

///////////////////////////////////////////////////////////////////////////////

void BlenderSceneExporter::attachMaterialToGeometry( Geometry* geometryEntity, uint materialIdx ) const
{
   ASSERT_MSG( materialIdx < m_materialDefinitions.size(), "Specified material wasn't exported" );
   if ( materialIdx >= m_materialDefinitions.size() )
   {
      return;
   }
   const MaterialDefinition& materialDef = *m_materialDefinitions[materialIdx];

   MaterialEntity* materialEntity = new MaterialEntity( materialDef.name );
   materialEntity->setMaterial( m_material );

   // setup the entity
   {
      // surface properties setup
      SurfaceProperties& surfaceProperties = materialEntity->accessSurfaceProperties();
      surfaceProperties.setAmbientColor( materialDef.ambientColor );
      surfaceProperties.setDiffuseColor( materialDef.diffuseColor );
      surfaceProperties.setSpecularColor( materialDef.specularColor );

      // textures setup
      if ( materialDef.normalTextureIndex >= 0 && materialDef.normalTextureIndex < (int)m_textures.size() )
      {
         Texture* normalTexture = m_textures[ materialDef.normalTextureIndex ];
         materialEntity->setTexture( MT_NORMALS, normalTexture );
      }

      for ( int i = MT_DIFFUSE_1; i < MT_DIFFUSE_2; ++i )
      {
         int mapIdx = i - MT_DIFFUSE_1;
         if ( mapIdx < materialDef.diffuseTexturesCount )
         {
            int diffuseTextureIdx = materialDef.diffuseTexturesIndices[mapIdx];
            Texture* diffuseTexture = m_textures[ diffuseTextureIdx ];
            materialEntity->setTexture( (MaterialTextures)i, diffuseTexture );
         }
      }
   }

   geometryEntity->add( materialEntity );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BlenderSceneExporter::MaterialDefinition::MaterialDefinition( const TamyMaterial& rhs )
{
   int nameLen = strlen( rhs.name ) + 1;
   name = new char[nameLen];
   strcpy_s( name, nameLen, rhs.name );

   ambientColor = rhs.ambientColor;
   diffuseColor = rhs.diffuseColor;
   specularColor = rhs.specularColor;
   normalTextureIndex = rhs.normalTextureIndex;

   diffuseTexturesCount = rhs.diffuseTexturesCount;
   if ( diffuseTexturesCount > 0 )
   {
      diffuseTexturesIndices = new int[ diffuseTexturesCount ];
      memcpy( diffuseTexturesIndices, rhs.diffuseTexturesIndices, sizeof( int ) * diffuseTexturesCount );
   }
   else
   {
      diffuseTexturesIndices = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

BlenderSceneExporter::MaterialDefinition::~MaterialDefinition()
{
   delete [] name;
   name = NULL;

   delete [] diffuseTexturesIndices;
   diffuseTexturesIndices = NULL;
}

///////////////////////////////////////////////////////////////////////////////
