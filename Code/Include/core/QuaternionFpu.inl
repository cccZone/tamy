#ifndef _QUATERNION_H
#error "This file can be included only in Quaternion.h"
#else

#include "core\MathDefs.h"
#include "core\Vector.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

Quaternion::Quaternion()
{
   memcpy( m_quad.v, Quad_0001.v.v, sizeof( QuadStorage ) );
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::setIdentity()
{
   memcpy( m_quad.v, Quad_0001.v.v, sizeof( QuadStorage ) );
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::set( float x, float y, float z, float w )
{
   m_quad.v[0] = x;
   m_quad.v[1] = y;
   m_quad.v[2] = z;
   m_quad.v[3] = w;
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::setAxisAngle( const Vector& axis, float angle )
{
   ASSERT_MSG( axis.isNormalized(), "Axis vector is not unit length" );

   float s = sin( angle/2 );
   float x = axis.x * s;
   float y = axis.y * s; 
   float z = axis.z * s;
   float w = cos( angle/2 );
   set( x, y, z, w );
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::operator=( const Quaternion& rhs )
{
   memcpy( m_quad.v, rhs.m_quad.v, sizeof( QuadStorage ) );
}

///////////////////////////////////////////////////////////////////////////////

bool Quaternion::operator==( const Quaternion& rhs ) const
{
   int result = memcmp( m_quad.v, rhs.m_quad.v, sizeof( QuadStorage ) );
   return result == 0;
}

///////////////////////////////////////////////////////////////////////////////

bool Quaternion::operator!=( const Quaternion& rhs ) const
{
   int result = memcmp( m_quad.v, rhs.m_quad.v, sizeof( QuadStorage ) );
   return result != 0;
}

///////////////////////////////////////////////////////////////////////////////

float& Quaternion::operator[]( int idx )
{
   return m_quad.v[idx];
}

///////////////////////////////////////////////////////////////////////////////

float Quaternion::operator[]( int idx ) const
{
   return m_quad.v[idx];
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::setAdd( const Quaternion& q1, const Quaternion& q2 )
{
   m_quad.v[0] = q1.m_quad.v[0] + q2.m_quad.v[0];
   m_quad.v[1] = q1.m_quad.v[1] + q2.m_quad.v[1];
   m_quad.v[2] = q1.m_quad.v[2] + q2.m_quad.v[2];
   m_quad.v[3] = q1.m_quad.v[3] + q2.m_quad.v[3];
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::setSub( const Quaternion& q1, const Quaternion& q2 )
{
   m_quad.v[0] = q1.m_quad.v[0] - q2.m_quad.v[0];
   m_quad.v[1] = q1.m_quad.v[1] - q2.m_quad.v[1];
   m_quad.v[2] = q1.m_quad.v[2] - q2.m_quad.v[2];
   m_quad.v[3] = q1.m_quad.v[3] - q2.m_quad.v[3];
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::setMul( const Quaternion& q1, const Quaternion& q2 )
{
   // x  =  x2 * w1  +  w2 * x1  +  y2 * z1  -  z2 * y1
   // y  =  w2 * y1  +  y2 * w1  -  x2 * z1  +  z2 * x1
   // z  =  w2 * z1  +  z2 * w1  +  x2 * y1  -  y2 * x1
   // w  =  w2 * w1  - 
   m_quad.v[0] = q2.m_quad.v[0]*q1.m_quad.v[3] + q2.m_quad.v[3]*q1.m_quad.v[0] + q2.m_quad.v[1]*q1.m_quad.v[2] - q2.m_quad.v[2]*q1.m_quad.v[1];
   m_quad.v[1] = q2.m_quad.v[3]*q1.m_quad.v[1] - q2.m_quad.v[0]*q1.m_quad.v[2] + q2.m_quad.v[1]*q1.m_quad.v[3] + q2.m_quad.v[2]*q1.m_quad.v[0];
   m_quad.v[2] = q2.m_quad.v[3]*q1.m_quad.v[2] + q2.m_quad.v[0]*q1.m_quad.v[1] - q2.m_quad.v[1]*q1.m_quad.v[0] + q2.m_quad.v[2]*q1.m_quad.v[3];
   m_quad.v[3] = q2.m_quad.v[3]*q1.m_quad.v[3] - q2.m_quad.v[0]*q1.m_quad.v[0] - q2.m_quad.v[1]*q1.m_quad.v[1] - q2.m_quad.v[2]*q1.m_quad.v[2];
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::add( const Quaternion& rhs )
{
   m_quad.v[0] += rhs.m_quad.v[0];
   m_quad.v[1] += rhs.m_quad.v[1];
   m_quad.v[2] += rhs.m_quad.v[2];
   m_quad.v[3] += rhs.m_quad.v[3];
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::sub( const Quaternion& rhs )
{
   m_quad.v[0] -= rhs.m_quad.v[0];
   m_quad.v[1] -= rhs.m_quad.v[1];
   m_quad.v[2] -= rhs.m_quad.v[2];
   m_quad.v[3] -= rhs.m_quad.v[3];
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::mul( const Quaternion& rhs )
{
   float x = rhs.m_quad.v[0]*m_quad.v[3] + rhs.m_quad.v[3]*m_quad.v[0] + rhs.m_quad.v[1]*m_quad.v[2] - rhs.m_quad.v[2]*m_quad.v[1];
   float y = rhs.m_quad.v[3]*m_quad.v[1] - rhs.m_quad.v[0]*m_quad.v[2] + rhs.m_quad.v[1]*m_quad.v[3] + rhs.m_quad.v[2]*m_quad.v[0];
   float z = rhs.m_quad.v[3]*m_quad.v[2] + rhs.m_quad.v[0]*m_quad.v[1] - rhs.m_quad.v[1]*m_quad.v[0] + rhs.m_quad.v[2]*m_quad.v[3];
   float w = rhs.m_quad.v[3]*m_quad.v[3] - rhs.m_quad.v[0]*m_quad.v[0] - rhs.m_quad.v[1]*m_quad.v[1] - rhs.m_quad.v[2]*m_quad.v[2];

   m_quad.v[0] = x;
   m_quad.v[1] = y;
   m_quad.v[2] = z;
   m_quad.v[3] = w;
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::neg()
{
   m_quad.v[0] = -m_quad.v[0];
   m_quad.v[1] = -m_quad.v[1];
   m_quad.v[2] = -m_quad.v[2];
   m_quad.v[3] = -m_quad.v[3];
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::setConjugated( const Quaternion& rhs )
{
   m_quad.v[0] = -rhs.m_quad.v[0];
   m_quad.v[1] = -rhs.m_quad.v[1];
   m_quad.v[2] = -rhs.m_quad.v[2];
   m_quad.v[3] =  rhs.m_quad.v[3];
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::conjugate()
{
   m_quad.v[0] = -m_quad.v[0];
   m_quad.v[1] = -m_quad.v[1];
   m_quad.v[2] = -m_quad.v[2];
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::normalize()
{
   float len = sqrt( m_quad.v[0]*m_quad.v[0] + m_quad.v[1]*m_quad.v[1] + m_quad.v[2]*m_quad.v[2] + m_quad.v[3]*m_quad.v[3] );
   if ( len > 0.0f )
   {
      m_quad.v[0] /= len;
      m_quad.v[1] /= len;
      m_quad.v[2] /= len;
      m_quad.v[3] /= len;
   }
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::setSlerp( const Quaternion& a, const Quaternion& b, const FastFloat& t )
{
   // Calculate angle between them.
   double cosHalfTheta = a.m_quad.v[3] * b.m_quad.v[3] + a.m_quad.v[0] * b.m_quad.v[0] + a.m_quad.v[1] * b.m_quad.v[1] + a.m_quad.v[2] * b.m_quad.v[2];

   // if a=b or a=-b then theta = 0 and we can return a
   if ( abs( cosHalfTheta ) >= 1.0 )
   {
      m_quad.v[3] = a.m_quad.v[3];
      m_quad.v[0] = a.m_quad.v[0];
      m_quad.v[1] = a.m_quad.v[1];
      m_quad.v[2] = a.m_quad.v[2];
      return;
   }

   // Calculate temporary values.
   double halfTheta = acos( cosHalfTheta );
   double sinHalfTheta = sqrt( 1.0 - cosHalfTheta*cosHalfTheta );

   // if theta = 180 degrees then result is not fully defined
   // we could rotate around any axis normal to a or b
   if ( fabs( sinHalfTheta ) < 0.001 )
   { 
      // fabs is floating point absolute
      m_quad.v[3] = (float)( a.m_quad.v[3] * 0.5 + b.m_quad.v[3] * 0.5 );
      m_quad.v[0] = (float)( a.m_quad.v[0] * 0.5 + b.m_quad.v[0] * 0.5 );
      m_quad.v[1] = (float)( a.m_quad.v[1] * 0.5 + b.m_quad.v[1] * 0.5 );
      m_quad.v[2] = (float)( a.m_quad.v[2] * 0.5 + b.m_quad.v[2] * 0.5 );

      return;
   }

   double ratioA = sin( ( 1 - t.m_val ) * halfTheta ) / sinHalfTheta;
   double ratioB = sin( t.m_val * halfTheta ) / sinHalfTheta; 

   m_quad.v[3] = (float)( a.m_quad.v[3] * ratioA + b.m_quad.v[3] * ratioB );
   m_quad.v[0] = (float)( a.m_quad.v[0] * ratioA + b.m_quad.v[0] * ratioB );
   m_quad.v[1] = (float)( a.m_quad.v[1] * ratioA + b.m_quad.v[1] * ratioB );
   m_quad.v[2] = (float)( a.m_quad.v[2] * ratioA + b.m_quad.v[2] * ratioB );
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::getAxis( Vector& outAxis ) const
{
   float sq = (float)sqrt( 1.0f - m_quad.v[3]*m_quad.v[3] );
   if ( sq != 0.0f )
   {
      outAxis.x = m_quad.v[0] / sq;
      outAxis.y = m_quad.v[1] / sq;
      outAxis.z = m_quad.v[2] / sq;
      outAxis.normalize();
   }
}

///////////////////////////////////////////////////////////////////////////////

float Quaternion::getAngle() const
{
   return 2.0f * (float)acos( m_quad.v[3] );
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::transform( const Vector& inVec, Vector& outVec ) const
{
   float x = m_quad.v[0];
   float y = m_quad.v[1];
   float z = m_quad.v[2];
   float w = m_quad.v[3];
   float x2 = x*x;
   float y2 = y*y;
   float z2 = z*z;
   float w2 = w*w;

   outVec.x = w2*inVec.x + 2*y*w*inVec.z - 2*z*w*inVec.y + x2*inVec.x + 2*y*x*inVec.y + 2*z*x*inVec.z - z2*inVec.x - y2*inVec.x;
   outVec.y = 2*x*y*inVec.x + y2*inVec.y + 2*z*y*inVec.z + 2*w*z*inVec.x - z2*inVec.y + w2*inVec.y - 2*x*w*inVec.z - x2*inVec.y;
   outVec.z = 2*x*z*inVec.x + 2*y*z*inVec.y + z2*inVec.z - 2*w*y*inVec.x - y2*inVec.z + 2*w*x*inVec.y - x2*inVec.z + w2*inVec.z;
}

///////////////////////////////////////////////////////////////////////////////


#endif // _QUATERNION_H
