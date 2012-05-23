#include "core\EulerAngles.h"
#include "core\Vector.h"
#include "core\Matrix.h"
#include "core\Quaternion.h"
#include "core\OutStream.h"
#include "core\InStream.h"
#include "core\MathDefs.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

EulerAngles::EulerAngles()
   : yaw(0)
   , pitch(0)
   , roll(0)
{
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles::EulerAngles( float _yaw, float _pitch, float _roll )
   : yaw(_yaw)
   , pitch(_pitch)
   , roll(_roll)
{
   normalize();
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::setFromShortestRotation( const Vector& vec1, const Vector& vec2 )
{
   ASSERT_MSG( vec1.isNormalized(), "vec1 needs to be normalized in order for setFromShortestRotation to work" );
   ASSERT_MSG( vec2.isNormalized(), "vec2 needs to be normalized in order for setFromShortestRotation to work" );

   float angle = acos( vec1.dot( vec2 ) );
   Vector axis;

   if( fabs( angle ) < 1e-4 )
   {
      angle = 0;
      axis = Vector::OZ;
   }
   else if ( fabs( angle - M_PI ) < 1e-3 )
   { 
      axis.setCross( vec1, vec2 );
      axis.normalize();
   }
   else
   {
      axis.setCross( vec1, vec2 );
      axis.normalize();
   }

   Quaternion quat;
   quat.setAxisAngle( axis, angle );
   setFromQuaternion( quat );

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::setFromQuaternion( const Quaternion& q )
{
   float ww = q.w*q.w;
   float xx = q.x*q.x;
   float yy = q.y*q.y;
   float zz = q.z*q.z;
   float xy = q.x*q.y;
   float zw = q.z*q.w;
   float yw = q.y*q.w;
   float yz = q.y*q.z;
   float xw = q.x*q.w;
   float xz = q.x*q.z;

   const float polarityTolerance = 1e-5f;

   float polarityFactor = yw - xz;
   if ( 0.5f - polarityFactor <= polarityTolerance )
   {
      // singularity at south pole
      roll = atan2( q.x , q.w );
      yaw = (float)M_PI / 2.0f;
      pitch = 0.0f;
   }
   else if ( polarityFactor + 0.5f <= polarityTolerance )
   {
      // singularity at north pole
      roll = atan2( q.x , q.w );
      yaw = (float)-M_PI / 2.0f;
      pitch = 0.0f;
      pitch = 0;
   }
   else
   {

      // pitch works fine in range <-180, 180 >, but yaw and roll don't
      pitch = atan2( 2.0f * ( xw + yz ), 1.0f - 2.0f * ( xx + yy ) );  // theta1
      yaw = asin( 2.0f * polarityFactor ); // theta2
      roll = atan2( 2.0f * ( zw + xy ), 1.0f - 2.0f * ( yy + zz ) ); // theta3
   }

   yaw = RAD2DEG( yaw );
   pitch = RAD2DEG( pitch );
   roll = RAD2DEG( roll );

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

bool EulerAngles::operator==( const EulerAngles& rhs ) const
{
   return (yaw == rhs.yaw) && (pitch == rhs.pitch) && (roll == rhs.roll);
}

///////////////////////////////////////////////////////////////////////////////

bool EulerAngles::operator!=( const EulerAngles& rhs ) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator=( const EulerAngles& rhs )
{
   yaw = rhs.yaw;
   pitch = rhs.pitch;
   roll = rhs.roll;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator+( const EulerAngles& rhs ) const
{
   return EulerAngles(yaw + rhs.yaw, pitch + rhs.pitch, roll + rhs.roll);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator+=( const EulerAngles& rhs )
{
   yaw += rhs.yaw;
   pitch += rhs.pitch;
   roll += rhs.roll;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator-( const EulerAngles& rhs ) const
{
   return EulerAngles(yaw - rhs.yaw, pitch - rhs.pitch, roll - rhs.roll);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator-=( const EulerAngles& rhs )
{
   yaw -= rhs.yaw;
   pitch -= rhs.pitch;
   roll -= rhs.roll;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator*( const EulerAngles& rhs ) const
{
   return EulerAngles(yaw * rhs.yaw, pitch * rhs.pitch, roll * rhs.roll);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator*=( const EulerAngles& rhs )
{
   yaw *= rhs.yaw;
   pitch *= rhs.pitch;
   roll *= rhs.roll;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator/( const EulerAngles& rhs ) const
{
   return EulerAngles(yaw / rhs.yaw, pitch / rhs.pitch, roll / rhs.roll);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator/=( const EulerAngles& rhs )
{
   yaw /= rhs.yaw;
   pitch /= rhs.pitch;
   roll /= rhs.roll;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator+( float val ) const
{
   return EulerAngles(yaw + val, pitch + val, roll + val);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator+=(float val)
{
   yaw += val;
   pitch += val;
   roll += val;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator-(float val) const
{
   return EulerAngles(yaw - val, pitch - val, roll - val);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator-=(float val)
{
   yaw -= val;
   pitch -= val;
   roll -= val;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator*(float val) const
{
   return EulerAngles(yaw * val, pitch * val, roll * val);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator*=(float val)
{
   yaw *= val;
   pitch *= val;
   roll *= val;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator/(float val) const
{
   return EulerAngles(yaw / val, pitch / val, roll / val);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator/=(float val)
{
   yaw /= val;
   pitch /= val;
   roll /= val;
   return *this;
}


///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::normalized() const
{
   EulerAngles o(yaw, pitch, roll);
   o.normalize();
   return o;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::normalize()
{
   yaw -= (int)(yaw / 360) * 360;
   pitch -= (int)(pitch / 360) * 360;
   roll -= (int)(roll / 360) * 360;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& stream, const EulerAngles& rhs)
{
   stream << "<"  << rhs.yaw
          << ", " << rhs.pitch
          << ", " << rhs.roll
          << ">";
   return stream;
}

///////////////////////////////////////////////////////////////////////////////

OutStream& operator<<( OutStream& serializer, const EulerAngles& rhs )
{
   serializer << rhs.yaw;
   serializer << rhs.pitch;
   serializer << rhs.roll;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////

InStream& operator>>( InStream& serializer, EulerAngles& rhs )
{
   serializer >> rhs.yaw;
   serializer >> rhs.pitch;
   serializer >> rhs.roll;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////
