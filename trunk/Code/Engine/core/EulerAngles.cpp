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
