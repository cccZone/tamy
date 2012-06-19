#include "core\Quaternion.h"
#include "core\EulerAngles.h"
#include "core\OutStream.h"
#include "core\InStream.h"
#include "core\MathDefs.h"
#include "core\Vector.h"
#include "core\VectorUtil.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

Quaternion  Quaternion::IDENTITY( 0.0f, 0.0f, 0.0f, 1.0f );

///////////////////////////////////////////////////////////////////////////////

Quaternion::Quaternion()
   : x(0)
   , y(0)
   , z(0) 
   , w(0)
{
}

///////////////////////////////////////////////////////////////////////////////

Quaternion::Quaternion( float _x, float _y, float _z, float _w )
   : x(_x)
   , y(_y)
   , z(_z) 
   , w(_w)
{
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::operator=( const Quaternion& rhs )
{
   x = rhs.x;
   y = rhs.y;
   z = rhs.z;
   w = rhs.w;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

bool Quaternion::operator==( const Quaternion& rhs ) const
{
   return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

///////////////////////////////////////////////////////////////////////////////

bool Quaternion::operator!=( const Quaternion& rhs ) const
{
   return !( *this == rhs );
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::setAdd( const Quaternion& q1, const Quaternion& q2 )
{
   x = q1.x + q2.x;
   y = q1.y + q2.y;
   z = q1.z + q2.z;
   w = q1.w + q2.w;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::setSub( const Quaternion& q1, const Quaternion& q2 )
{
   x = q1.x - q2.x;
   y = q1.y - q2.y;
   z = q1.z - q2.z;
   w = q1.w - q2.w;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::setMul( const Quaternion& q1, const Quaternion& q2 )
{
   x = q2.x*q1.w + q2.w*q1.x + q2.y*q1.z - q2.z*q1.y;
   y = q2.w*q1.y - q2.x*q1.z + q2.y*q1.w + q2.z*q1.x;
   z = q2.w*q1.z + q2.x*q1.y - q2.y*q1.x + q2.z*q1.w;
   w = q2.w*q1.w - q2.x*q1.x - q2.y*q1.y - q2.z*q1.z;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::add( const Quaternion& rhs )
{
   x += rhs.x;
   y += rhs.y;
   z += rhs.z;
   w += rhs.w;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::sub( const Quaternion& rhs )
{
   x -= rhs.x;
   y -= rhs.y;
   z -= rhs.z;
   w -= rhs.w;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::mul( const Quaternion& rhs )
{
   float _x = rhs.x*w + rhs.w*x + rhs.y*z - rhs.z*y;
   float _y = rhs.w*y - rhs.x*z + rhs.y*w + rhs.z*x;
   float _z = rhs.w*z + rhs.x*y - rhs.y*x + rhs.z*w;
   float _w = rhs.w*w - rhs.x*x - rhs.y*y - rhs.z*z;

   x = _x;
   y = _y;
   z = _z;
   w = _w;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::neg()
{
   x = -x;
   y = -y;
   z = -z;
   w = -w;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::setConjugated( const Quaternion& rhs )
{
   x = -rhs.x;
   y = -rhs.y;
   z = -rhs.z;
   w = rhs.w;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::conjugate()
{
   x = -x;
   y = -y;
   z = -z;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::setFromEulerAngles( const EulerAngles& angles )
{
   // Q(Roll) * Q(Pitch) * Q(Yaw) - to be in sync with the counterpart method Matrix::setRotation( const EulerAngles& angles )

   const double radYaw = DEG2RAD( angles.yaw * 0.5f );
   const double radPitch = DEG2RAD( angles.pitch * 0.5f );
   const double radRoll = DEG2RAD( angles.roll * 0.5f );

   double c1 = cos( radYaw );
   double s1 = sin( radYaw );
   double c2 = cos( radRoll );
   double s2 = sin( radRoll );
   double c3 = cos( radPitch );
   double s3 = sin( radPitch );

   double c2s3 = c2*s3;
   double c2c3 = c2*c3;
   double s2s3 = s2*s3;
   double s2c3 = s2*c3;
   x = (float)( c1*c2s3 + s1*s2c3 );
   y = (float)( s1*c2c3 - c1*s2s3 );
   z = (float)( c1*s2c3 - s1*c2s3 );
   w = (float)( c1*c2c3 + s1*s2s3 );

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::setAxisAngle( const Vector& axis, float angle )
{
   ASSERT_MSG( axis.isNormalized(), "Axis vector is not unit length" );

   float s = sin( angle/2 );
   x = axis.x * s;
   y = axis.y * s; 
   z = axis.z * s;
   w = cos( angle/2 );

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::setFromShortestRotation( const Vector& v1, const Vector& v2 )
{
   float d = v1.dot( v2 ); 

   Vector axis;
   axis.setCross( v1, v2 );

   float qw = (float)sqrt( v1.lengthSq() * v2.lengthSq() ) + d;
   if ( qw < 0.0001f ) 
   { 
      // vectors are 180 degrees apart
      w = 0.0f;
      x = -v1.z;
      y = v1.y;
      z = v1.x;
   } 
   else
   {
      w = qw;
      x = axis.x;
      y = axis.y;
      z = axis.z;
   }

   // normalize the quaternion
   normalize();

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::setSlerp( const Quaternion& a, const Quaternion& b, float t )
{
   // Calculate angle between them.
   double cosHalfTheta = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;

   // if a=b or a=-b then theta = 0 and we can return a
   if ( abs( cosHalfTheta ) >= 1.0 )
   {
      w = a.w;
      x = a.x;
      y = a.y;
      z = a.z;
      return *this;
   }

   // Calculate temporary values.
   double halfTheta = acos( cosHalfTheta );
   double sinHalfTheta = sqrt( 1.0 - cosHalfTheta*cosHalfTheta );

   // if theta = 180 degrees then result is not fully defined
   // we could rotate around any axis normal to a or b
   if ( fabs( sinHalfTheta ) < 0.001 )
   { 
      // fabs is floating point absolute
      w = (float)( a.w * 0.5 + b.w * 0.5 );
      x = (float)( a.x * 0.5 + b.x * 0.5 );
      y = (float)( a.y * 0.5 + b.y * 0.5 );
      z = (float)( a.z * 0.5 + b.z * 0.5 );

      return *this;
   }

   double ratioA = sin( ( 1 - t ) * halfTheta ) / sinHalfTheta;
   double ratioB = sin( t * halfTheta ) / sinHalfTheta; 

   w = (float)( a.w * ratioA + b.w * ratioB );
   x = (float)( a.x * ratioA + b.x * ratioB );
   y = (float)( a.y * ratioA + b.y * ratioB );
   z = (float)( a.z * ratioA + b.z * ratioB );

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::normalize()
{
   float len = sqrt( x*x + y*y + z*z + w*w );
   if ( len > 0.0f )
   {
      x /= len;
      y /= len;
      z /= len;
      w /= len;
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::getAxis( Vector& outAxis ) const
{
   float sq = (float)sqrt( 1.0f - w*w );
   if ( sq != 0.0f )
   {
      outAxis.x = x / sq;
      outAxis.y = y / sq;
      outAxis.z = z / sq;
      outAxis.normalize();
   }
}

///////////////////////////////////////////////////////////////////////////////

float Quaternion::getAngle() const
{
   return 2.0f * (float)acos( w );
}

///////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::removeAxisComponent( const Vector& axis )
{
   // rotate the specified axis
   Vector rotatedAxis;
   transform( axis, rotatedAxis );

   float dot = axis.dot( rotatedAxis );
   if ( dot - 1.0f > -1e-3f )
   {
      // the axis is the main rotation axis, so there's nothing
      // left from the quaternion's rotation now
      *this = Quaternion::IDENTITY;
   }
   else if ( dot + 1.0f < 1e-3f )
   {
      // the axis goes in the opposite direction
      Vector perpVector;
      VectorUtil::calculatePerpendicularVector( axis, perpVector );
      perpVector.normalize();
      
      x = perpVector.x;
      y = perpVector.y;
      z = perpVector.z;
      w = 0.0f;
   }
   else
   {
      const float rotationAngle = acos( dot );

      Vector rotationAxis;
      rotationAxis.setCross( axis, rotatedAxis ).normalize();
      setAxisAngle(rotationAxis, rotationAngle);
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

float Quaternion::decompose( const Vector& decompositionAxis, Quaternion& outRemainingQuaternion ) const
{
   ASSERT_MSG( decompositionAxis.isNormalized(), "The decomposition axis must be a unit-length vector." );
   
   Quaternion axisRot;
   {
      outRemainingQuaternion = *this;
      outRemainingQuaternion.removeAxisComponent( decompositionAxis );

      // axisRot = inv(rest) * q
      Quaternion inv_rest;
      inv_rest.setConjugated( outRemainingQuaternion );
      axisRot.setMul( inv_rest, *this );
   }

   float a = axisRot.getAngle();
   return a;
}

///////////////////////////////////////////////////////////////////////////////

void Quaternion::transform( const Vector& inVec, Vector& outVec ) const
{
   float x2 = x*x;
   float y2 = y*y;
   float z2 = z*z;
   float w2 = w*w;

   outVec.x = w2*inVec.x + 2*y*w*inVec.z - 2*z*w*inVec.y + x2*inVec.x + 2*y*x*inVec.y + 2*z*x*inVec.z - z2*inVec.x - y2*inVec.x;
   outVec.y = 2*x*y*inVec.x + y2*inVec.y + 2*z*y*inVec.z + 2*w*z*inVec.x - z2*inVec.y + w2*inVec.y - 2*x*w*inVec.z - x2*inVec.y;
   outVec.z = 2*x*z*inVec.x + 2*y*z*inVec.y + z2*inVec.z - 2*w*y*inVec.x - y2*inVec.z + 2*w*x*inVec.y - x2*inVec.z + w2*inVec.z;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<( std::ostream& stream, const Quaternion& rhs )
{
   stream << "[" << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << "]";
   return stream;
}

///////////////////////////////////////////////////////////////////////////////

OutStream& operator<<( OutStream& serializer, const Quaternion& rhs )
{
   serializer << rhs.x;
   serializer << rhs.y;
   serializer << rhs.z;
   serializer << rhs.w;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////

InStream& operator>>( InStream& serializer, Quaternion& rhs )
{
   serializer >> rhs.x;
   serializer >> rhs.y;
   serializer >> rhs.z;
   serializer >> rhs.w;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////
