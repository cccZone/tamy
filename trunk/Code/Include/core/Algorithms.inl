#ifndef _ALGORITHMS_H
#error "This file can only be included from Algorithms.h"
#else


///////////////////////////////////////////////////////////////////////////////

template< typename T >
void Collection::clear( T& collection )
{
   for ( T::iterator it = collection.begin(); it != collection.end(); ++it )
   {
      delete *it;
   }
   collection.clear();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
T clamp( T val, T min, T max )
{
   return ( min < val ) ? ( ( val < max ) ? val : max ) : min;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
T roundEx( T value, T precision )
{
   T roundedVal = (T)(long)( value / precision ) * precision;

   return roundedVal;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
float mag( T value )
{
   if ( value == 0.0 )
   {
      return 1.0f;
   }
   else
   {
      int intMag = (int)log10( value );
      if ( intMag > 0 )
      {
         return (float)pow( 10.0, intMag );
      }
      else if ( intMag == 0 )
      {
         if ( value >= 1.0 )
         {
            return 1.0f;
         }
         else
         {
            return 0.1f;
         }
      }
      else // intMag < 0
      {
         intMag = -intMag;
         return (float)( 1.0 / pow( 10.0, intMag ) );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _ALGORITHMS_H
