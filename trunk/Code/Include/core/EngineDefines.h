/// @file   core/EngineDefines.h
/// @brief  here's where all major engine-level macros and definitions are stored
#ifndef _ENGINE_DEFINES_H
#define _ENGINE_DEFINES_H


///////////////////////////////////////////////////////////////////////////////

 #define _USE_FAST_ARRAYS

///////////////////////////////////////////////////////////////////////////////

#define _USE_SIMD

#define HK_SSE_VERSION 0x20

#ifdef _USE_SIMD
   // include SSE
   #include <xmmintrin.h>

   #if HK_SSE_VERSION >= 0x20
   #include <emmintrin.h>
   #endif

   #if HK_SSE_VERSION >= 0x30
   #include <pmmintrin.h>
   #endif

   #if HK_SSE_VERSION >= 0x31
   #include <tmmintrin.h>
   #endif

   #if HK_SSE_VERSION >= 0x40
   #include <wmmintrin.h>
   #endif

   #if HK_SSE_VERSION >= 0x41
   #include <smmintrin.h>
   #endif

   #if HK_SSE_VERSION >= 0x42
   #include <nmmintrin.h>
   #endif

   #if HK_SSE_VERSION >= 0x50
   #include <immintrin.h>
   #endif
#endif // _USE_SIMD

///////////////////////////////////////////////////////////////////////////////

#endif // _ENGINE_DEFINES_H 
