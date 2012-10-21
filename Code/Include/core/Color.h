/// @file   core\Color.h
/// @brief  color representation
#pragma once

#include "core\types.h"
#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

class OutStream;
class InStream;

///////////////////////////////////////////////////////////////////////////////

/**
 * A support structure describing an RGBA color.
 */
struct Color
{
   DECLARE_ALLOCATOR( Color, AM_ALIGNED_16 );

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
    * Conversion operator to a DWORD representation.
    */
   operator unsigned long() const;

   /**
    * Sets the color value from an internet-friendly representation.
    *
    * @param val
    */
   void setFromLong( ulong val );

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend OutStream& operator<<( OutStream& serializer, const Color& color );
   friend InStream& operator>>( InStream& serializer, Color& color );
};

///////////////////////////////////////////////////////////////////////////////
