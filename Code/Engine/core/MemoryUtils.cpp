#include "core.h"
#include "core/MemoryUtils.h"
#include <memory>


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

bool MemoryUtils::isAddressAligned( void* ptr, uint alignment )
{
   ulong specifiedAddr = (ulong)ptr;
   ulong alignedAddr = ( specifiedAddr + sizeof( void* ) + alignment - 1 ) & ~( alignment - 1 );

   return ( ( specifiedAddr - alignedAddr ) % alignment ) == 0;
}

///////////////////////////////////////////////////////////////////////////////
