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

#endif // _ALGORITHMS_H
