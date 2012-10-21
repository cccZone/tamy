#ifndef _FAST_FLOAT_H
#error "This file can only be included from FastFloat.h"
#else

#include "core\MathDataStorage.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

void FastFloat::operator=( const FastFloat& rhs )
{
   m_val = rhs.m_val;
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::operator=( const FastFloatStorage& rhs )
{
   m_val = rhs;
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setFromFloat( float val )
{
   m_val = _mm_set1_ps( val );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setFromInt( int val )
{
   m_val = _mm_cvtepi32_ps( _mm_set1_epi32(val) );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setFromByte( byte val )
{
   int intVal = val;
   m_val = _mm_cvtepi32_ps( _mm_set1_epi32( intVal ) );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setFromWord( word val )
{
   int intVal = val;
   m_val = _mm_cvtepi32_ps( _mm_set1_epi32( intVal ) );
}

///////////////////////////////////////////////////////////////////////////////

float FastFloat::getFloat() const
{
   float s;
   _mm_store_ss( &s, m_val );
   return s;
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setAdd( const FastFloat& a, const FastFloat& b )
{
   m_val = _mm_add_ps( a.m_val, b.m_val );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setSub( const FastFloat& a, const FastFloat& b )
{
   m_val = _mm_sub_ps( a.m_val, b.m_val );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setMul( const FastFloat& a, const FastFloat& b )
{
   m_val = _mm_mul_ps( a.m_val, b.m_val );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setDiv( const FastFloat& a, const FastFloat& b )
{
   m_val = _mm_div_ps( a.m_val, b.m_val );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::add( const FastFloat& a )
{
   m_val = _mm_add_ps( m_val, a.m_val );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::sub( const FastFloat& a )
{
   m_val = _mm_sub_ps( m_val, a.m_val );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::mul( const FastFloat& a )
{
   m_val = _mm_mul_ps( m_val, a.m_val );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::div( const FastFloat& a )
{
   m_val = _mm_div_ps( m_val, a.m_val );
}

///////////////////////////////////////////////////////////////////////////////

bool FastFloat::operator<( const FastFloat& rhs ) const
{
   return _mm_ucomilt_ss( m_val, rhs.m_val ) == 1;
}

///////////////////////////////////////////////////////////////////////////////

bool FastFloat::operator<=( const FastFloat& rhs ) const
{
   return _mm_ucomile_ss( m_val, rhs.m_val ) == 1;
}

///////////////////////////////////////////////////////////////////////////////

bool FastFloat::operator>( const FastFloat& rhs ) const
{
   return _mm_ucomigt_ss( m_val, rhs.m_val ) == 1;
}

///////////////////////////////////////////////////////////////////////////////

bool FastFloat::operator>=( const FastFloat& rhs ) const
{
   return _mm_ucomige_ss( m_val, rhs.m_val ) == 1;
}

///////////////////////////////////////////////////////////////////////////////

bool FastFloat::operator==( const FastFloat& rhs ) const
{
   return _mm_ucomieq_ss( m_val, rhs.m_val ) == 1;
}

///////////////////////////////////////////////////////////////////////////////

bool FastFloat::operator!=( const FastFloat& rhs ) const
{
   return _mm_ucomineq_ss( m_val, rhs.m_val ) == 1;
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setMax( const FastFloat& a, const FastFloat& b )
{
   m_val = _mm_max_ps( a.m_val, b.m_val );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setMin( const FastFloat& a, const FastFloat& b )
{
   m_val = _mm_min_ps( a.m_val, b.m_val );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setClamped( const FastFloat& a, const FastFloat& minVal, const FastFloat& maxVal )
{
   FastFloatStorage mv = _mm_max_ps( a.m_val, minVal.m_val );
   m_val = _mm_min_ps( maxVal.m_val, mv );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setAbs( const FastFloat& a )
{
   static ALIGN_16 const uint absValMask[4] = { 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff };
   m_val = _mm_and_ps( a.m_val, *( const FastFloatStorage*)&absValMask );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::abs()
{
   static ALIGN_16 const uint absValMask[4] = { 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff };
   m_val = _mm_and_ps( m_val, *( const __m128* )&absValMask );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setFlipSign( const FastFloat& a, const FastFloat& sign )
{
   static ALIGN_16 const uint signMask[4] = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
   m_val = _mm_xor_ps( a.m_val, _mm_and_ps( sign.m_val, *( const __m128* )&signMask ) );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setSign( const FastFloat& a )
{
   static ALIGN_16 const uint signMask[4]  = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
   m_val = _mm_xor_ps( Float_1.m_val, _mm_and_ps( a.m_val, *( const __m128* )&signMask ) );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::sign()
{
   static ALIGN_16 const uint signmask[4] = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
   m_val = _mm_xor_ps( Float_1.m_val, _mm_and_ps( m_val, *( const __m128* )&signmask ) );
}

///////////////////////////////////////////////////////////////////////////////

bool FastFloat::approxEqual( const FastFloat& a, const FastFloat& eps ) const
{
   static ALIGN_16 const uint absValMask[4] = { 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff };
   __m128 diff = _mm_sub_ps( m_val, a.m_val );
   diff = _mm_and_ps( diff, *( const __m128* )&absValMask );
   return _mm_ucomile_ss( diff, eps.m_val ) == 1;
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setSqrt( const FastFloat& a )
{
   const __m128 equalsZero = _mm_cmple_ps( a.m_val, _mm_setzero_ps() );
   const __m128 e = _mm_sqrt_ps( a.m_val );
   m_val = _mm_andnot_ps( equalsZero, e );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::sqrt()
{
   const __m128 equalsZero = _mm_cmple_ps( m_val, _mm_setzero_ps() );
   const __m128 e = _mm_sqrt_ps( m_val );
   m_val = _mm_andnot_ps( equalsZero, e );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setReciprocal( const FastFloat& a )
{
   m_val = _mm_div_ps( Float_1.m_val, a.m_val );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::reciprocal()
{
   m_val = _mm_div_ps( Float_1.m_val, m_val );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setNeg( const FastFloat& a )
{
   static ALIGN_16 const uint signMask[4] = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
   m_val = _mm_xor_ps( a.m_val, *( const FastFloatStorage*)&signMask );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::neg()
{
   static ALIGN_16 const uint signMask[4] = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
   m_val = _mm_xor_ps( m_val, *( const FastFloatStorage*)&signMask );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::sin()
{
   float v = ::sin( m_val.m128_f32[0] );
   m_val = _mm_set1_ps( v );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::cos()
{
   float v = ::cos( m_val.m128_f32[0] );
   m_val = _mm_set1_ps( v );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setSin( const FastFloat& val )
{
   float v = ::sin( val.m_val.m128_f32[0] );
   m_val = _mm_set1_ps( v );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setCos( const FastFloat& val )
{
   float v = ::cos( val.m_val.m128_f32[0] );
   m_val = _mm_set1_ps( v );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::asin()
{
   float v = ::asin( m_val.m128_f32[0] );
   m_val = _mm_set1_ps( v );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::acos()
{
   float v = ::acos( m_val.m128_f32[0] );
   m_val = _mm_set1_ps( v );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setAsin( const FastFloat& val )
{
   float v = ::asin( val.m_val.m128_f32[0] );
   m_val = _mm_set1_ps( v );
}

///////////////////////////////////////////////////////////////////////////////

void FastFloat::setAcos( const FastFloat& val )
{
   float v = ::acos( val.m_val.m128_f32[0] );
   m_val = _mm_set1_ps( v );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _FAST_FLOAT_H
