#include "ml-Blender/PythonInterface.h"
#include "ml-Blender/BlenderSceneExporter.h"
#include "ml-Blender/TamyMaterial.h"
#include "ml-Blender/TamyMesh.h"
#include "ml-Blender/TamyScene.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

void begin_export( const char* filesystemRoot, const char* exportDir, int entitiesCount )
{
   // reset the export tool's contents, so that we start fresh
   BlenderSceneExporter::getInstance().initialize( filesystemRoot, exportDir, entitiesCount );
}

///////////////////////////////////////////////////////////////////////////////

void export_textures( TamyTexture* arrTextures, int texturesCount )
{
   BlenderSceneExporter& exporter = BlenderSceneExporter::getInstance();

   for ( int i = 0; i < texturesCount; ++i )
   {
      exporter.createTexture( arrTextures[i] );
   }
}

///////////////////////////////////////////////////////////////////////////////

void export_materials( TamyMaterial* arrMaterials, int materialsCount )
{
   BlenderSceneExporter& exporter = BlenderSceneExporter::getInstance();
   exporter.storeMaterials( arrMaterials, materialsCount );
}

///////////////////////////////////////////////////////////////////////////////

void export_meshes( TamyMesh* arrMeshes, int meshesCount )
{
   BlenderSceneExporter& exporter = BlenderSceneExporter::getInstance();

   for ( int i = 0; i < meshesCount; ++i )
   {
      exporter.createMesh( arrMeshes[i] );
   }
}

///////////////////////////////////////////////////////////////////////////////

void export_geometry_entity( TamyGeometry geometry )
{
   BlenderSceneExporter& exporter = BlenderSceneExporter::getInstance();
   SpatialEntity* entity = exporter.createGeometryEntity( geometry );

   Matrix globalMtx;
   geometry.localMatrix.applyTo( globalMtx );
   exporter.addEntity( entity, globalMtx, geometry.parentIdx );
}

///////////////////////////////////////////////////////////////////////////////

void export_light_entity( TamyLight light )
{
   BlenderSceneExporter& exporter = BlenderSceneExporter::getInstance();
   SpatialEntity* entity = exporter.createLightEntity( light );

   Matrix globalMtx;
   light.localMatrix.applyToLight( globalMtx );
   exporter.addEntity( entity, globalMtx, light.parentIdx );
}

///////////////////////////////////////////////////////////////////////////////

void export_world_settings( TamyWorld world )
{
   BlenderSceneExporter& exporter = BlenderSceneExporter::getInstance();
   exporter.setupWorld( world );
}


///////////////////////////////////////////////////////////////////////////////

void assemble_scene( const char* sceneName )
{
   BlenderSceneExporter& exporter = BlenderSceneExporter::getInstance();

   exporter.assembleScene( sceneName );
}

///////////////////////////////////////////////////////////////////////////////



// TODO:
// I> GEOMETRIA
// 1. export bryly miekkiej    ( done )
// 2. export bryly twarde      ( done )
// 3. export dwoch odrebnych bryl   ( done )
// 4. export dwoch bryl polaczyonych w hierarchie ( done )
//
// MATERIALY
// 1. export dwoch bryl, kazdej z innym materialem ( done )
// 2. export tekstury diffuse ( done )
// 3. export tekstury normalek ( done )
// 4. export tekstury specular ( i wsparcie w modelu oswietlenia ) ( done )
// 5. wsparcie dla dokladnych materialow ( kolor specular'a itd ) = http://http.developer.nvidia.com/GPUGems/gpugems_ch18.html

// INNE ELEMENTY SCENY
// 1. swiatla (done )
// 2. ustawienia point light'a ( done )
// 3. oswietlenie ambientowe z ustawien swiata (done)