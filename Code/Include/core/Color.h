#pragma once

/// @file   core\Color.h
/// @brief  color representation

#include "core\Class.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Serializer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A support structure describing an RGBA color.
 */
struct Color : public RTTIObject
{
   DECLARE_RTTI_STRUCT

   union
   {
      struct
      {
         float r;
         float g;
         float b;
         float a;
      };

      float c[ 4 ];
   };

   /**
    * Default constructor,
    */
   Color();

   /**
    * Constructor.
    */
   Color(float _r, float _g, float _b, float _a);

   bool operator==(const Color& rhs) const;

   bool operator!=(const Color& rhs) const;

   Color operator+(const Color& lhs) const;

   /**
    * Conversion operator to a dx-native type.
    */
   operator D3DXVECTOR4() const { return D3DXVECTOR4( c ); }

   friend Serializer& operator<<(Serializer& serializer, Color& color);
};

///////////////////////////////////////////////////////////////////////////////
