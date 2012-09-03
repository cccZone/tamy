#include "ml-Blender/PythonInterface.h"
#include "ml-Blender/BlenderSceneExporter.h"
#include "ml-Blender/TamyMaterial.h"
#include "ml-Blender/TamyMesh.h"
#include "ml-Blender/TamyScene.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

void begin_export( const char* filesystemRoot, const char* exportDir )
{
   // reset the export tool's contents, so that we start fresh
   BlenderSceneExporter::getInstance().initialize( filesystemRoot, exportDir );
}

///////////////////////////////////////////////////////////////////////////////

void end_export()
{
   // we're done with the export - cleanup
   BlenderSceneExporter::getInstance().reset();
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

void export_scene( TamyScene scene )
{
   BlenderSceneExporter& exporter = BlenderSceneExporter::getInstance();

   exporter.createScene( scene );
}

///////////////////////////////////////////////////////////////////////////////



// TODO:
// I> GEOMETRIA
// 1. export bryly miekkiej    ( done )
// 2. export bryly twarder
// 3. export dwoch odrebnych bryl
// 4. export dwoch bryl polaczyonych w hierarchie
//
// MATERIALY
// 1. export dwoch bryl, kazdej z innym materialem
// 2. export tekstury diffuse
// 3. export tekstury normalek
// 4. export tekstury specular ( i wsparcie w modelu oswietlenia )