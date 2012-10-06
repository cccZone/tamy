/// @file   core/SimdUtils.h
/// @brief  SIMD utility functions
#ifndef _SIMD_UTILS_H
#define _SIMD_UTILS_H

#include "core\EngineDefines.h"

#ifdef _USE_SIMD


///////////////////////////////////////////////////////////////////////////////

#define PACK_SIMD( x, y, z, w )  _mm_movelh_ps( _mm_unpacklo_ps( x, y ), _mm_unpacklo_ps( z, w ) )

///////////////////////////////////////////////////////////////////////////////

/**
 * SIMD utility functions.
 */
class SimdUtils
{
public:
 
   // -------------------------------------------------------------------------
   // Vector operations
   // -------------------------------------------------------------------------
   /**
    * Calculates a dot product of the specified quads, taking its first N components into account.
    *
    * @param N
    * @param vecA
    * @param vecB
    * @param outDot
    */
   template< int N >
   static void dot( const __m128* vecA, const __m128* vecB, __m128* outDot );

   /**
    * Calculates a dot product of the specified quads.
    *
    * @param vecA
    * @param vecB
    * @param outCross
    */
   static void cross( const __m128* vecA, const __m128* vecB, __m128* outCross );

   /**
    * Normalizes the specified vector, taking its first N components into account.
    *
    * @param N
    * @param vec
    * @param outNormalVec
    */
   template< int N >
   static void normalize( const __m128* vec, __m128* outNormalVec );

   // -------------------------------------------------------------------------
   // Arithmetic operations
   // -------------------------------------------------------------------------
   /**
    * outResult = a + t * b
    *
    * @param quadA
    * @param t
    * @param quadB
    * @param outCross
    */
   static void addMul( const __m128* quadA, const __m128* t, const __m128* quadB, __m128* outResult );

   /**
    * this = sign >= 0 ? a : -a
    *
    * @param quad
    * @param mask
    * @param outQuad
    */
   static void flipSign( const __m128* quad, const __m128* mask, __m128* outQuad );

   /**
    * Calculates a square root of the specified quad, and returns its reciprocal.
    *
    * @param quad
    * @param outQuad
    */
   static void sqrtInverse( const __m128* quad, __m128* outQuad );


   // -------------------------------------------------------------------------
   // Conversion operations
   // -------------------------------------------------------------------------

   /**
    * Converts 4 floats to a SIMD value.
    *
    * @param val
    */
   static void toSimd( float a, float b, float c, float d, __m128* outSimdVal );

   /**
    * Converts a float value to a SIMD quad.
    *
    * @param val     float value
    * @return        SIMD value
    */
   static const __m128 fromFloat( float val );

   /**
    * Combines two quads, one of which will contribute to XYZ part, and the other to the W part
    * of the output quad
    *
    * @param xyz
    * @param w
    * @param outQuad
    */
   static void setXYZ_W( const __m128* xyz, const __m128* w, __m128* outQuad );

   /**
    * Extracts the specified quad component and initialized a whole other quad with it.
    *
    * @param quad
    * @param idx              component index
    * @param outComponent
    */
   static void getComponent( const __m128* quad, const int idx, __m128* outComponent );

   // -------------------------------------------------------------------------
   // Masking operations
   // -------------------------------------------------------------------------
   /**
    * outMask = quadA < quadB
    */
   static void lessMask( const __m128* quadA, const __m128* quadB, __m128* outMask );

   /**
    * outMask = quadA <= quadB
    */
   static void lessEqualMask( const __m128* quadA, const __m128* quadB, __m128* outMask );

   /**
    * outMask = quadA > quadB
    */
   static void greaterMask( const __m128* quadA, const __m128* quadB, __m128* outMask );

   /**
    * outMask = quadA >= quadB
    */
   static void greaterEqualMask( const __m128* quadA, const __m128* quadB, __m128* outMask );

   /**
    * outMask = quadA == quadB
    */
   static void equalMask( const __m128* quadA, const __m128* quadB, __m128* outMask );

   /**
    * outMask = quadA != quadB
    */
   static void notEqualMask( const __m128* quadA, const __m128* quadB, __m128* outMask );

   /**
    * outMask = quad < 0
    */
   static void lessZeroMask( const __m128* quad, __m128* outMask );

   /**
    * outMask = quad <= 0
    */
   static void lessEqualZeroMask( const __m128* quad, __m128* outMask );

   /**
    * outMask = quad > 0
    */
   static void greaterZeroMask( const __m128* quad, __m128* outMask );

   /**
    * outMask = quad >= 0
    */
   static void greaterEqualZeroMask( const __m128* quad, __m128* outMask );

   /**
    * outMask = quad == 0
    */
   static void equalZeroMask( const __m128* quad, __m128* outMask );

   /**
    * outMask = quad != 0
    */
   static void notEqualZeroMask( const __m128* quad, __m128* outMask );
};

///////////////////////////////////////////////////////////////////////////////

#include "core\SimdUtils.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _USE_SIMD
#endif // _SIMD_UTILS_H
