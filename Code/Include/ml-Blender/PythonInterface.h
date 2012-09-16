/// @file   ml-Blender/PythonInterface.h
/// @brief  a C-interface that can be accessed from a Python code using 'ctypes' tools
#pragma once

#include "ml-Blender/DllDefines.h"


///////////////////////////////////////////////////////////////////////////////

struct TamyTexture;
struct TamyMaterial;
struct TamyScene;
struct TamyGeometry;
struct TamyMesh;
struct TamyLight;
struct TamyWorld;

///////////////////////////////////////////////////////////////////////////////

/**
 * Begins the export procedure.
 */
ML_BLENDER_API void begin_export( const char* filesystemRoot, const char* exportDir, int entitiesCount );

///////////////////////////////////////////////////////////////////////////////

/**
 * Exports a set of textures from Blender to Tamy.
 */
ML_BLENDER_API void export_textures( TamyTexture* arrTextures, int texturesCount );

///////////////////////////////////////////////////////////////////////////////

/**
 * Exports a material from Blender to Tamy.
 */
ML_BLENDER_API void export_materials( TamyMaterial* arrMaterials, int materialsCount );

///////////////////////////////////////////////////////////////////////////////

/**
 * Exports a set of meshes from Blender to Tamy.
 */
ML_BLENDER_API void export_meshes( TamyMesh* arrMeshes, int meshesCount );

///////////////////////////////////////////////////////////////////////////////

/**
 * Exports a single Geometry entity from Blender to Tamy.
 */
ML_BLENDER_API void export_geometry_entity( TamyGeometry geometry );

///////////////////////////////////////////////////////////////////////////////

/**
 * Exports a single Light entity from Blender to Tamy.
 */
ML_BLENDER_API void export_light_entity( TamyLight light );

///////////////////////////////////////////////////////////////////////////////

/**
 * Exports the settings of the world ( ambient lighting setup, fog, sky data and so on )
 */
ML_BLENDER_API void export_world_settings( TamyWorld world );

///////////////////////////////////////////////////////////////////////////////

/**
 * Called once all entities have been exported. It assembles the scene using those exported entities.
 */
ML_BLENDER_API void assemble_scene( const char* sceneName );

///////////////////////////////////////////////////////////////////////////////
