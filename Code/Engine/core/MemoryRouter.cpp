#include "core.h"
#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

MemoryRouter* MemoryRouter::s_theInstance = NULL;

///////////////////////////////////////////////////////////////////////////////

MemoryRouter& MemoryRouter::getInstance()
{
   if ( !s_theInstance )
   {
      s_theInstance = new MemoryRouter();
   }
   return *s_theInstance;
}

///////////////////////////////////////////////////////////////////////////////

void* MemoryRouter::alloc( size_t size, AllocationMode allocMode, MemoryAllocator* allocator )
{
   const size_t headerSize = sizeof( void* );

   const int ALIGNMENT = 16;
   size_t alignedSize = calcAlignedSize( size, ALIGNMENT );
   void* pa = allocator->alloc( alignedSize + headerSize );

   if( !pa )
   {
      return NULL;
   }

   // first - insert the header
   *(int*)pa = (int)allocator;
   pa = (char*)pa + headerSize;

   // then align the address
   void* ptr = MemoryUtils::alignPointer( pa, ALIGNMENT );
         

   return ptr;
}

///////////////////////////////////////////////////////////////////////////////

void MemoryRouter::dealloc( void* ptr, AllocationMode allocMode )
{
   const size_t headerSize = sizeof( void* );

   void* postHeaderPtr = MemoryUtils::resolveAlignedPointer( ptr );

   // decode the address of the allocator
   void* origPtr = (char*)postHeaderPtr - headerSize;
   int allocAddrRaw = *(int*)origPtr;
   MemoryAllocator* allocator = (MemoryAllocator*)allocAddrRaw;

   if ( origPtr )
   {
      allocator->dealloc( origPtr );
   }
}

///////////////////////////////////////////////////////////////////////////////

void* MemoryRouter::convertAllocatedToObjectAddress( void* allocatedAddr, AllocationMode allocMode )
{
   const size_t headerSize = sizeof( void* );
   const int ALIGNMENT = 16;

   void* retAddress = (char*)allocatedAddr + headerSize;

   // then align the address
   retAddress = MemoryUtils::alignPointer( retAddress, ALIGNMENT );
   return retAddress;
}

///////////////////////////////////////////////////////////////////////////////
