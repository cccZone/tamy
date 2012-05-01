#pragma once

/// @file   core\Color.h
/// @brief  color representation

#include "core\ReflectionObject.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class OutStream;
class InStream;

///////////////////////////////////////////////////////////////////////////////

/**
 * A support structure describing an RGBA color.
 */
struct Color : public ReflectionObject
{
   DECLARE_STRUCT()

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
    * Constructor ( using float values from range <0, 1> )
    */
   Color( float _r, float _g, float _b, float _a );

   /**
    * Constructor ( using integer values from range <0, 255> )
    */
   Color( unsigned char _r, unsigned char _g, unsigned char _b );

   bool operator==(const Color& rhs) const;

   bool operator!=(const Color& rhs) const;

   Color operator+(const Color& lhs) const;

   /**
    * Conversion operator to a dx-native type.
    */
   operator D3DXVECTOR4() const { return D3DXVECTOR4( c ); }

   /**
    * Conversion operator to a DWORD representation.
    */
   operator unsigned long() const;

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend OutStream& operator<<( OutStream& serializer, Color& color );
   friend InStream& operator>>( InStream& serializer, Color& color );
};

///////////////////////////////////////////////////////////////////////////////
