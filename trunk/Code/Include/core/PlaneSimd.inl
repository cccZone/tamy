#ifndef _PLANE_H
#error "This file can only be included from Plane.h"
#else

#include "core\Vector.h"
#include "core\SimdUtils.h"


///////////////////////////////////////////////////////////////////////////////

void Plane::setZero()
{
   m_quad = _mm_setzero_ps();
}

///////////////////////////////////////////////////////////////////////////////

void Plane::set( const Vector& vec )
{
   SimdUtils::toSimd( vec.x, vec.y, vec.z, vec.w, &m_quad );
}

///////////////////////////////////////////////////////////////////////////////

void Plane::set( const Vector& normal, const FastFloat& distance )
{
   SimdUtils::toSimd( normal.x, normal.y, normal.z, distance.m_val.m128_f32[3], &m_quad ); 
}

///////////////////////////////////////////////////////////////////////////////

void Plane::set( const FastFloat& a, const FastFloat& b, const FastFloat& c, const FastFloat& d )
{
   m_quad = PACK_SIMD( a.m_val, b.m_val, c.m_val, d.m_val );
}

///////////////////////////////////////////////////////////////////////////////

void Plane::operator=( const Plane& rhs )
{
   m_quad = rhs.m_quad;
}

///////////////////////////////////////////////////////////////////////////////

bool Plane::operator==( const Plane& rhs ) const
{
   __m128 cmpResult = _mm_cmpeq_ps( m_quad, rhs.m_quad );
   int vmask = _mm_movemask_ps( cmpResult );
   return vmask == 0xf;
}

///////////////////////////////////////////////////////////////////////////////

bool Plane::operator!=( const Plane& rhs ) const
{
   __m128 cmpResult = _mm_cmpeq_ps( m_quad, rhs.m_quad );
   int vmask = _mm_movemask_ps( cmpResult );
   return vmask == 0x0;
}

///////////////////////////////////////////////////////////////////////////////

float& Plane::operator[]( int idx )
{
   return m_quad.m128_f32[idx];
}

///////////////////////////////////////////////////////////////////////////////

float Plane::operator[]( int idx ) const
{
   return m_quad.m128_f32[idx];
}

///////////////////////////////////////////////////////////////////////////////

void Plane::normalize()
{
   __m128 dot;
   SimdUtils::dot<3>( &m_quad, &m_quad, &dot );

   // at this point we have a squared length of the plane normal - so calculate its square root
   __m128 equalsZero = _mm_cmple_ps( dot, _mm_setzero_ps() );
   __m128 sqrtDot = _mm_sqrt_ps( dot );
   __m128 length = _mm_andnot_ps( equalsZero, sqrtDot );

   m_quad = _mm_div_ps( m_quad, length );
}

///////////////////////////////////////////////////////////////////////////////

const FastFloat Plane::dotCoord( const Vector& coord ) const
{
   // <fastfloat.todo> this will be taken directly from vector's SIMD representation
   __m128 vecQuad;
   SimdUtils::toSimd( coord.x, coord.y, coord.z, coord.w, &vecQuad );

   // plane.x * coord.x, plane.y * coord.y, plane.z * coord.z, plane.w * 1
   static ALIGN_16 const uint discardWMask[4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000 };
   static ALIGN_16 const uint setWMask[4] = { 0x00000000, 0x00000000, 0x00000000, 0x3f800000 };
   vecQuad = _mm_and_ps( vecQuad, *( const __m128* )&discardWMask );
   vecQuad = _mm_or_ps( vecQuad, *( const __m128* )&setWMask );

   __m128 dot;
   SimdUtils::dot<4>( &m_quad, &vecQuad, &dot );

   return FastFloat( dot );
}

///////////////////////////////////////////////////////////////////////////////

const FastFloat Plane::dotNormal( const Vector& normal ) const
{
   // <fastfloat.todo> this will be taken directly from vector's SIMD representation
   __m128 vecQuad;
   SimdUtils::toSimd( normal.x, normal.y, normal.z, normal.w, &vecQuad );

   __m128 dot;
   SimdUtils::dot<3>( &vecQuad, &m_quad, &dot );

   return FastFloat( dot );
}

///////////////////////////////////////////////////////////////////////////////

void Plane::getNormal( Vector& outPlaneNormal ) const
{
   outPlaneNormal.x = m_quad.m128_f32[0];
   outPlaneNormal.y = m_quad.m128_f32[1];
   outPlaneNormal.z = m_quad.m128_f32[2];
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PLANE_H
