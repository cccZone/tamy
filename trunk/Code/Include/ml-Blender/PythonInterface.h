/// @file   ml-Blender/PythonInterface.h
/// @brief  a C-interface that can be accessed from a Python code using 'ctypes' tools
#pragma once

#include "ml-Blender/DllDefines.h"


///////////////////////////////////////////////////////////////////////////////

struct TamyTexture;
struct TamyMaterial;
struct TamyScene;
struct TamyMesh;

///////////////////////////////////////////////////////////////////////////////

/**
 * Begins the export procedure.
 */
ML_BLENDER_API void begin_export( const char* filesystemRoot, const char* exportDir );

///////////////////////////////////////////////////////////////////////////////

/**
 * Ends the export procedure and cleans up after itself.
 */
ML_BLENDER_API void end_export();

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
 * Exports an entire scene from Blender to Tamy.
 */
ML_BLENDER_API void export_scene( TamyScene scene );

///////////////////////////////////////////////////////////////////////////////
