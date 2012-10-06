#include "core\DefaultAllocator.h"
#include "core\MemoryUtils.h"
#include <stdio.h>
#include <stdlib.h>


///////////////////////////////////////////////////////////////////////////////

void* DefaultAllocator::alloc( size_t size )
{
   void* ptr = MemoryUtils::mallocAligned( size, MemoryUtils::ALIGNMENT );
   return ptr;
}

///////////////////////////////////////////////////////////////////////////////

void DefaultAllocator::dealloc( void* ptr )
{
   MemoryUtils::freeAligned( ptr );
}

///////////////////////////////////////////////////////////////////////////////
