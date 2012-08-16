#include "core\Plane.h"
#include "core\Vector.h"
#include "core\OutStream.h"
#include "core\InStream.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

Plane::Plane()
   : a( 0.0f )
   , b( 0.0f )
   , c( 0.0f )
   , d( 0.0f )
{
}

///////////////////////////////////////////////////////////////////////////////

Plane::Plane( float _a, float _b, float _c, float _d )
   : a( _a )
   , b( _b )
   , c( _c )
   , d( _d )
{
}

///////////////////////////////////////////////////////////////////////////////

Plane& Plane::operator=( const Plane& rhs )
{
   a = rhs.a;
   b = rhs.b;
   c = rhs.c;
   d = rhs.d;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

bool Plane::operator==( const Plane& rhs ) const
{
   return a == rhs.a && b == rhs.b && c == rhs.c && d == rhs.d;
}

///////////////////////////////////////////////////////////////////////////////

bool Plane::operator!=( const Plane& rhs ) const
{
   return !( *this == rhs );
}

///////////////////////////////////////////////////////////////////////////////

Plane& Plane::set( float _a, float _b, float _c, float _d )
{
   a = _a;
   b = _b;
   c = _c;
   d = _d;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Plane& Plane::setFromPointNormal( const Vector& point, const Vector& normal )
{
   d = -normal.dot( point );
   a = normal.x;
   b = normal.y;
   c = normal.z;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Plane& Plane::setFromPoints( const Vector& p1, const Vector& p2, const Vector& p3 )
{
   Vector edge1, edge2;
   edge1.setSub( p2, p1 );
   edge2.setSub( p3, p1 );

   Vector normal;
   normal.setCross( edge1, edge2 ).normalize();

   d = -normal.dot( p1 );
   a = normal.x;
   b = normal.y;
   c = normal.z;


   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Plane& Plane::normalize()
{
   float length = sqrt( a*a + b*b + c*c );
   if ( length != 0.0f )
   {
      a /= length;
      b /= length;
      c /= length;
      d /= length;
   }
   
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

float Plane::dotCoord( const Vector& coord ) const
{
   float t = coord.x * a + coord.y * b + coord.z * c + d;
   return t;
}

///////////////////////////////////////////////////////////////////////////////

float Plane::dotNormal( const Vector& normal ) const
{
   float t = normal.x * a + normal.y * b + normal.z * c;
   return t;
}

///////////////////////////////////////////////////////////////////////////////

void Plane::getNormal( Vector& outPlaneNormal ) const
{
   memcpy( outPlaneNormal.v.v, p, sizeof( float ) * 3 );
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<( std::ostream& stream, const Plane& rhs )
{
   stream << "[" << rhs.a << ", " << rhs.b << ", " << rhs.c << ", " << rhs.d << "]";
   return stream;
}

///////////////////////////////////////////////////////////////////////////////

OutStream& operator<<( OutStream& serializer, const Plane& rhs )
{
   serializer << rhs.a;
   serializer << rhs.b;
   serializer << rhs.c;
   serializer << rhs.d;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////

InStream& operator>>( InStream& serializer, Plane& rhs )
{
   serializer >> rhs.a;
   serializer >> rhs.b;
   serializer >> rhs.c;
   serializer >> rhs.d;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////
