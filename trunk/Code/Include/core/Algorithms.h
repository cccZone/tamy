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

#include "core/Algorithms.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _ALGORITHMS_H