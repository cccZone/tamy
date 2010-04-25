#pragma once

/// @file   core\Color.h
/// @brief  color representation


///////////////////////////////////////////////////////////////////////////////

class Serializer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A support structure describing an RGBA color.
 */
struct Color
{
   float r;
   float g;
   float b;
   float a;

   Color();
   Color(float _r, float _g, float _b, float _a);

   bool operator==(const Color& rhs) const;

   bool operator!=(const Color& rhs) const;

   Color operator+(const Color& lhs) const;

   void save(Serializer& serializer);
   void load(Serializer& serializer);
};

///////////////////////////////////////////////////////////////////////////////
