#include "core.h"
#include "core\DefaultAllocator.h"
#include "core\MemoryUtils.h"
#include <stdio.h>
#include <stdlib.h>


///////////////////////////////////////////////////////////////////////////////

DefaultAllocator::DefaultAllocator()
   : m_allocatedMemorySize( 0 )
{
}

///////////////////////////////////////////////////////////////////////////////

void* DefaultAllocator::alloc( size_t size )
{
   void* ptr = ::malloc( size );
   m_allocatedMemorySize += size;
   return ptr;
}

///////////////////////////////////////////////////////////////////////////////

void DefaultAllocator::dealloc( void* ptr )
{
   ulong allocatedSize = *( (ulong*)ptr - 4 );
   m_allocatedMemorySize -= allocatedSize;

   ::free( ptr );
}

///////////////////////////////////////////////////////////////////////////////
