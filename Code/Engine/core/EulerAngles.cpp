#include "core\EulerAngles.h"
#include "core\Vector.h"
#include "core\Matrix.h"

#define _USE_MATH_DEFINES
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

#define M2_PI  (float)(2.f * M_PI)

///////////////////////////////////////////////////////////////////////////////

BEGIN_RTTI(EulerAngles)
END_RTTI

///////////////////////////////////////////////////////////////////////////////

EulerAngles::EulerAngles()
: yaw(0)
, pitch(0)
, roll(0)
{
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles::EulerAngles(float _yaw, float _pitch, float _roll)
: yaw(_yaw)
, pitch(_pitch)
, roll(_roll)
{
   normalize();
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles::EulerAngles(const Vector& vec1, const Vector& vec2)
{
   Vector nVec1 = vec1.normalized();
   Vector nVec2 = vec2.normalized();
   float angle = acos( nVec1.dot( nVec2 ) );
   Vector axis;

   if( fabs( angle ) < 1e-4 )
   {
      angle = 0;
      axis = Vector( 0, 0, 1 );
   }
   else if ( fabs( angle - M_PI ) < 1e-3 )
   { 
      axis = nVec1.cross(nVec2);
   }
   else
   {
      axis = nVec1.cross(nVec2);
   }

   Matrix mtx( axis, angle );
   *this = (EulerAngles)mtx;
}

///////////////////////////////////////////////////////////////////////////////

bool EulerAngles::operator==(const EulerAngles& rhs) const
{
   return (yaw == rhs.yaw) && (pitch == rhs.pitch) && (roll == rhs.roll);
}

///////////////////////////////////////////////////////////////////////////////

bool EulerAngles::operator!=(const EulerAngles& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator=(const EulerAngles& rhs)
{
   yaw = rhs.yaw;
   pitch = rhs.pitch;
   roll = rhs.roll;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator+(const EulerAngles& rhs) const
{
   return EulerAngles(yaw + rhs.yaw, pitch + rhs.pitch, roll + rhs.roll);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator+=(const EulerAngles& rhs)
{
   yaw += rhs.yaw;
   pitch += rhs.pitch;
   roll += rhs.roll;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator-(const EulerAngles& rhs) const
{
   return EulerAngles(yaw - rhs.yaw, pitch - rhs.pitch, roll - rhs.roll);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator-=(const EulerAngles& rhs)
{
   yaw -= rhs.yaw;
   pitch -= rhs.pitch;
   roll -= rhs.roll;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator*(const EulerAngles& rhs) const
{
   return EulerAngles(yaw * rhs.yaw, pitch * rhs.pitch, roll * rhs.roll);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator*=(const EulerAngles& rhs)
{
   yaw *= rhs.yaw;
   pitch *= rhs.pitch;
   roll *= rhs.roll;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator/(const EulerAngles& rhs) const
{
   return EulerAngles(yaw / rhs.yaw, pitch / rhs.pitch, roll / rhs.roll);
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles& EulerAngles::operator/=(const EulerAngles& rhs)
{
   yaw /= rhs.yaw;
   pitch /= rhs.pitch;
   roll /= rhs.roll;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

EulerAngles EulerAngles::operator+(float val) const
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

EulerAngles::operator D3DXQUATERNION() const
{
   const double radYaw = toRadians( yaw );
   const double radPitch = toRadians( pitch );
   const double radRoll = toRadians( roll );

   double c1 = cos( radYaw * 0.5f );
   double s1 = sin( radYaw * 0.5f );
   double c2 = cos( radPitch * 0.5f );
   double s2 = sin( radPitch * 0.5f );
   double c3 = cos( radRoll * 0.5f );
   double s3 = sin( radRoll * 0.5f );
   double c1c2 = c1*c2;
   double s1s2 = s1*s2;

   D3DXQUATERNION result;
   result.w = (float)( c1c2*c3 - s1s2*s3 );
   result.x = (float)( c1c2*s3 + s1s2*c3 );
   result.y = (float)( s1*c2*c3 + c1*s2*s3 );
   result.z = (float)( c1*s2*c3 - s1*c2*s3 );
   return result;
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

float EulerAngles::toDegrees(float radians)
{
   return (float)((radians * 180.f) / M_PI);
}

///////////////////////////////////////////////////////////////////////////////

float EulerAngles::toRadians(float degrees)
{
   return (float)((degrees * M_PI) / 180.f);
}

///////////////////////////////////////////////////////////////////////////////
