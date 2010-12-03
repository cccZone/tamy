/// @file   core/Algorithms.h
/// @brief  a set of algorithms facilitating code creation

#ifndef _ALGORITHMS_H
#define _ALGORITHMS_H


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

#include "core/Algorithms.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _ALGORITHMS_H