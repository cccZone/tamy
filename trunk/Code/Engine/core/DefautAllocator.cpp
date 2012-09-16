#include "core\DefaultAllocator.h"
#include <stdio.h>
#include <stdlib.h>


///////////////////////////////////////////////////////////////////////////////

void* DefaultAllocator::alloc( size_t size )
{
   void* ptr = malloc( size );
   return ptr;
}

///////////////////////////////////////////////////////////////////////////////

void DefaultAllocator::dealloc( void* ptr )
{
   free( ptr );
}

///////////////////////////////////////////////////////////////////////////////