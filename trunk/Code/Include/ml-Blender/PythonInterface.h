/// @file   ml-Blender/PythonInterface.h
/// @brief  a C-interface that can be accessed from a Python code using 'ctypes' tools
#pragma once


///////////////////////////////////////////////////////////////////////////////

#ifdef ML_BLENDER_EXPORTS
   #define ML_BLENDER_API extern "C" __declspec(dllexport)
#else
   #define ML_BLENDER_API extern "C" __declspec(dllimport)
#endif


///////////////////////////////////////////////////////////////////////////////

ML_BLENDER_API void se_printMsg( const char* msg );

///////////////////////////////////////////////////////////////////////////////
