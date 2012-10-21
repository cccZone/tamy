#include "core.h"
#include "core/FastFloat.h"
#include "core/MathDefs.h"


///////////////////////////////////////////////////////////////////////////////

FastFloat g_floatConstants[FloatMathConst_MAX] =
{
   FLOAT_CONSTANT( 0.0f ),
   FLOAT_CONSTANT( 1.0f ),
   FLOAT_CONSTANT( 2.0f ),
   FLOAT_CONSTANT( 3.0f ),
   FLOAT_CONSTANT( 4.0f ),
   FLOAT_CONSTANT( 5.0f ),
   FLOAT_CONSTANT( 6.0f ),
   FLOAT_CONSTANT( 7.0f ),
   FLOAT_CONSTANT( 8.0f ),
   FLOAT_CONSTANT( 9.0f ),

   FLOAT_CONSTANT( -1.0f ),
   FLOAT_CONSTANT( -2.0f ),

   FLOAT_CONSTANT( 0.1f ),
   FLOAT_CONSTANT( 0.01f ),
   FLOAT_CONSTANT( 0.001f ),
   FLOAT_CONSTANT( 0.0001f ),
   FLOAT_CONSTANT( -0.1f ),
   FLOAT_CONSTANT( -0.01f ),
   FLOAT_CONSTANT( -0.001f ),
   FLOAT_CONSTANT( -0.0001f ),

   FLOAT_CONSTANT( 1.0f/2.0f ),
   FLOAT_CONSTANT( 1.0f/3.0f ),
   FLOAT_CONSTANT( 1.0f/4.0f ),

   FLOAT_CONSTANT( -1.0f/2.0f ),
   FLOAT_CONSTANT( -1.0f/3.0f ),
   FLOAT_CONSTANT( -1.0f/4.0f ),

   FLOAT_CONSTANT( (float)M_PI ),
   FLOAT_CONSTANT( 180.f / (float)M_PI ),
   FLOAT_CONSTANT( (float)M_PI / 180.f ),

   FLOAT_CONSTANT( 3.402823466e+38F ),
   FLOAT_CONSTANT( -3.402823466e+38F ),
};

///////////////////////////////////////////////////////////////////////////////

const FastFloat FastFloat::fromFloat( float val )
{
   FastFloat f;
   f.setFromFloat( val );
   return f;
}

///////////////////////////////////////////////////////////////////////////////

const FastFloat FastFloat::fromInt( int val )
{
   FastFloat f;
   f.setFromInt( val );
   return f;
}

///////////////////////////////////////////////////////////////////////////////

const FastFloat FastFloat::fromByte( byte val )
{
   FastFloat f;
   f.setFromByte( val );
   return f;
}

///////////////////////////////////////////////////////////////////////////////

const FastFloat FastFloat::fromWord( word val )
{
   FastFloat f;
   f.setFromWord( val );
   return f;
}

///////////////////////////////////////////////////////////////////////////////
