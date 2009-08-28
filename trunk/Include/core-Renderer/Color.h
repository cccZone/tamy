#pragma once


///////////////////////////////////////////////////////////////////////////////

/**
 * A support structure describing an RGBA color
 */
struct Color
{
   float r;
   float g;
   float b;
   float a;

   Color() : r(0), g(0), b(0), a(0) {}
   Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}

   bool operator==(const Color& rhs) const
   {
      return (r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a);
   }

   bool operator!=(const Color& rhs) const
   {
      return !(*this == rhs);
   }

   Color operator+(const Color& lhs) const
   {
      Color newColor;
      newColor.r = r + lhs.r;
      newColor.g = g + lhs.g;
      newColor.b = b + lhs.b;
      newColor.a = a + lhs.a;
      return newColor;
   }
};

///////////////////////////////////////////////////////////////////////////////
