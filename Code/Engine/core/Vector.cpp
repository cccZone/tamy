#include "core\Vector.h"
#include "core\OutStream.h"
#include "core\InStream.h"


///////////////////////////////////////////////////////////////////////////////

Vector Vector::ZERO( 0.0f, 0.0f, 0.0f, 0.0f );
Vector Vector::ONE( 1.0f, 1.0f, 1.0f, 1.0f );
Vector Vector::OX( 1.0f, 0.0f, 0.0f, 0.0f );
Vector Vector::OY( 0.0f, 1.0f, 0.0f, 0.0f );
Vector Vector::OZ( 0.0f, 0.0f, 1.0f, 0.0f );
Vector Vector::OW( 0.0f, 0.0f, 0.0f, 1.0f );
Vector Vector::OX_NEG( -1.0f, 0.0f, 0.0f, 0.0f );
Vector Vector::OY_NEG( 0.0f, -1.0f, 0.0f, 0.0f );
Vector Vector::OZ_NEG( 0.0f, 0.0f, -1.0f, 0.0f );
Vector Vector::OW_NEG( 0.0f, 0.0f, 0.0f, -1.0f );

///////////////////////////////////////////////////////////////////////////////

Vector::Vector() 
   : x(0)
   , y(0)
   , z(0) 
   , w(0)
{}

///////////////////////////////////////////////////////////////////////////////

Vector::Vector( float _x, float _y, float _z, float _w ) 
   : x(_x)
   , y(_y)
   , z(_z) 
   , w(_w)
{}

///////////////////////////////////////////////////////////////////////////////

bool Vector::equals4( const Vector& rhs ) const
{
   return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}


///////////////////////////////////////////////////////////////////////////////

bool Vector::operator==( const Vector& rhs ) const
{
   return x == rhs.x && y == rhs.y && z == rhs.z;
}

///////////////////////////////////////////////////////////////////////////////

bool Vector::operator!=( const Vector& rhs ) const
{
   return !( *this == rhs );
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::operator=( const Vector& rhs )
{
   x = rhs.x;
   y = rhs.y;
   z = rhs.z;
   w = rhs.w;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::setMul( const Vector& vec, float t )
{
   x = vec.x * t;
   y = vec.y * t;
   z = vec.z * t;
   w = vec.w * t;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::setMul( const Vector& vec1, const Vector& vec2 )
{
   x = vec1.x * vec2.x;
   y = vec1.y * vec2.y;
   z = vec1.z * vec2.z;
   w = vec1.w * vec2.w;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::setMulAdd( const Vector& vec1, float t, const Vector& vec2 )
{
   x = vec1.x * t + vec2.x;
   y = vec1.y * t + vec2.y;
   z = vec1.z * t + vec2.z;
   w = vec1.w * t + vec2.w;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::setDiv( const Vector& vec1, const Vector& vec2 )
{
   x = vec1.x / vec2.x;
   y = vec1.y / vec2.y;
   z = vec1.z / vec2.z;
   w = vec1.w / vec2.w;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::setAdd( const Vector& vec1, const Vector& vec2 )
{
   x = vec1.x + vec2.x;
   y = vec1.y + vec2.y;
   z = vec1.z + vec2.z;
   w = vec1.w + vec2.w;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::setSub( const Vector& vec1, const Vector& vec2 )
{
   x = vec1.x - vec2.x;
   y = vec1.y - vec2.y;
   z = vec1.z - vec2.z;
   w = vec1.w - vec2.w;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::mul( float t )
{
   x *= t;
   y *= t;
   z *= t;
   w *= t;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::mul( const Vector& vec )
{
   x *= vec.x;
   y *= vec.y;
   z *= vec.z;
   w *= vec.w;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::div( const Vector& vec )
{
   x /= vec.x;
   y /= vec.y;
   z /= vec.z;
   w /= vec.w;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::add( const Vector& vec )
{
   x += vec.x;
   y += vec.y;
   z += vec.z;
   w += vec.w;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::sub( const Vector& vec )
{
   x -= vec.x;
   y -= vec.y;
   z -= vec.z;
   w -= vec.w;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::neg()
{
   x = -x;
   y = -y;
   z = -z;
   w = -w;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

float Vector::dot( const Vector& rhs ) const
{
   return x * rhs.x + y * rhs.y + z * rhs.z;
}

///////////////////////////////////////////////////////////////////////////////

float Vector::dot4( const Vector& rhs ) const
{
   return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::setCross( const Vector& v1, const Vector& v2 )
{
   x = v1.y * v2.z - v1.z * v2.y;
   y = v1.z * v2.x - v1.x * v2.z;
   z = v1.x * v2.y - v1.y * v2.x;
   w = 1.0f;
   
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::preCross( const Vector& rhs )
{
   Vector tmpVec;
   tmpVec.setCross( *this, rhs );

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::postCross( const Vector& rhs )
{
   Vector tmpVec;
   tmpVec.setCross( rhs, *this );
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::set( float _x, float _y, float _z, float _w )
{
   x = _x;
   y = _y;
   z = _z;
   w = _w;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::setNormalized( const Vector& rhs )
{
   setNormalized( rhs.x, rhs.y, rhs.z );

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::setNormalized( float _x, float _y, float _z )
{
   float len = sqrt( _x * _x + _y * _y + _z * _z );
   if (len == 0)
   {
      x = y = z = w = 0;
   }
   else
   {
      x = _x / len;
      y = _y / len;
      z = _z / len;
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::normalize()
{
   float len = sqrt(x * x + y * y + z * z);
   if (len == 0)
   {
      x = y = z = 0;
   }
   else
   {
      x /= len;
      y /= len;
      z /= len;
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::floor()
{
   x = floorf( x );
   y = floorf( y );
   z = floorf( z );
   w = floorf( w );

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Vector& Vector::setLerp( const Vector& a, const Vector& b, float t )
{
   x = a.x + ( b.x - a.x ) * t;
   y = a.y + ( b.y - a.y ) * t;
   z = a.z + ( b.z - a.z ) * t;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

float Vector::length() const
{
   return sqrt(x * x + y * y + z * z);
}

///////////////////////////////////////////////////////////////////////////////

float Vector::lengthSq() const
{
   return x * x + y * y + z * z;
}

///////////////////////////////////////////////////////////////////////////////

bool Vector::isNormalized() const
{
   return abs( lengthSq() - 1.0f ) < 1e-6;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<( std::ostream& stream, const Vector& rhs )
{
   stream << rhs.v;
   return stream;
}

///////////////////////////////////////////////////////////////////////////////

OutStream& operator<<( OutStream& serializer, const Vector& rhs )
{
   serializer << rhs.v;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////

InStream& operator>>( InStream& serializer, Vector& rhs )
{
   serializer >> rhs.v;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void Vector::store( TVector< 2 >& rawVector )
{
   rawVector.v[0] = x;
   rawVector.v[1] = y;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void Vector::store( TVector< 3 >& rawVector )
{
   rawVector.v[0] = x;
   rawVector.v[1] = y;
   rawVector.v[2] = z;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void Vector::store( TVector< 4 >& rawVector )
{
   rawVector.v[0] = x;
   rawVector.v[1] = y;
   rawVector.v[2] = z;
   rawVector.v[3] = w;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void Vector::load( const TVector< 2 >& rawVector )
{
   x = rawVector.v[0];
   y = rawVector.v[1];
}

///////////////////////////////////////////////////////////////////////////////

template<>
void Vector::load( const TVector< 3 >& rawVector )
{
   x = rawVector.v[0];
   y = rawVector.v[1];
   z = rawVector.v[2];
}

///////////////////////////////////////////////////////////////////////////////

template<>
void Vector::load( const TVector< 4 >& rawVector )
{
   x = rawVector.v[0];
   y = rawVector.v[1];
   z = rawVector.v[2];
   w = rawVector.v[3];
}

///////////////////////////////////////////////////////////////////////////////
