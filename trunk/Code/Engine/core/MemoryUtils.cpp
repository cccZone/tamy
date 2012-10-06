#include "core/MemoryUtils.h"
#include <memory>


///////////////////////////////////////////////////////////////////////////////

int MemoryUtils::ALIGNMENT = 16;

///////////////////////////////////////////////////////////////////////////////

void* MemoryUtils::mallocAligned( uint size, uint alignment )
{
   void *pa, *ptr;

   pa = ::malloc( ( size + alignment - 1) + sizeof( void* ) );

   if( !pa )
   {
      return NULL;
   }

   ptr = alignPointer( pa, alignment );
   return ptr;
}

///////////////////////////////////////////////////////////////////////////////

void MemoryUtils::freeAligned( void* ptr )
{
   void* origPtr = resolveAlignedPointer( ptr );
   if ( origPtr )
   {
      free( origPtr );
   }
}

///////////////////////////////////////////////////////////////////////////////

void* MemoryUtils::alignPointer( void* ptr, uint alignment )
{
   void* alignedPtr;
   alignedPtr = ( void* )( ( ( ulong )ptr + sizeof( void* ) + alignment - 1 ) & ~( alignment - 1 ) );

   *( (void **)alignedPtr - 1 ) = ptr;
   return alignedPtr;
}

///////////////////////////////////////////////////////////////////////////////

void* MemoryUtils::resolveAlignedPointer( void* alignedPtr )
{
   if ( alignedPtr )
   {
      return *( (void **)alignedPtr-1 );
   }
   else
   {
      return NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
