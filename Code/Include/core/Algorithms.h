/// @file   core/Algorithms.h
/// @brief  a set of algorithms facilitating code creation

#ifndef _ALGORITHMS_H
#define _ALGORITHMS_H

#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

class Collection
{
public:
   /**
    * Clears a collection.
    *
    * @param collection
    */
   template< typename C >
   static void clear( C& collection );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Clamps the value to the specified range.
 *
 * @param val
 * @param min
 * @param max
 */
template< typename T >
T clamp( T val, T min, T max );

///////////////////////////////////////////////////////////////////////////////

/**
 * Rounds up a value to the specified precision.
 *
 * @param   value
 * @param   precision
 * @return  rounded value
 */
template< typename T >
T roundEx( T value, T precision );

///////////////////////////////////////////////////////////////////////////////

/**
 * Calculates the magnitude of a number.
 *
 * @param   value
 * @return  value magnitude
 */
template< typename T >
float mag( T value );

///////////////////////////////////////////////////////////////////////////////


#include "core/Algorithms.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _ALGORITHMS_H