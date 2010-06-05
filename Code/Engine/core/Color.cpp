#include "core\Color.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RTTI(Color)
END_RTTI

///////////////////////////////////////////////////////////////////////////////

Color::Color() 
: r( 1 )
, g( 1 )
, b( 1 )
, a( 1 ) 
{}

///////////////////////////////////////////////////////////////////////////////

Color::Color( float _r, float _g, float _b, float _a ) 
: r( _r )
, g( _g )
, b( _b )
, a( _a ) 
{}

///////////////////////////////////////////////////////////////////////////////

Color::Color(unsigned char _r, unsigned char _g, unsigned char _b)
: r( _r / 255.f )
, g( _g / 255.f )
, b( _b / 255.f )
, a( 1 )
{}

///////////////////////////////////////////////////////////////////////////////

bool Color::operator==(const Color& rhs) const
{
   return (r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a);
}

///////////////////////////////////////////////////////////////////////////////

bool Color::operator!=(const Color& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

Color Color::operator+(const Color& lhs) const
{
   Color newColor;
   newColor.r = r + lhs.r;
   newColor.g = g + lhs.g;
   newColor.b = b + lhs.b;
   newColor.a = a + lhs.a;
   return newColor;
}

///////////////////////////////////////////////////////////////////////////////

Serializer& operator<<(Serializer& serializer, Color& color)
{
   serializer << color.r;
   serializer << color.g;
   serializer << color.b;
   serializer<< color.a;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////

Color::operator unsigned long() const
{
   unsigned long val;
   val = ( ( unsigned long )( a * 255.f ) << 24 ) + 
         ( ( unsigned long )( r * 255.f ) << 16 ) + 
         ( ( unsigned long )( g * 255.f ) << 8 ) + 
         ( unsigned long )( b * 255.f );
   return val;
}

///////////////////////////////////////////////////////////////////////////////
