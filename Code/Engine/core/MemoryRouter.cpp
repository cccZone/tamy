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
   void* ptr  = NULL;
   const size_t headerSize = sizeof( void* );

   switch( allocMode )
   {
   case AM_ALIGNED_16:
      {
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
         ptr = MemoryUtils::alignPointer( pa, ALIGNMENT );
         break;
      }


   case AM_DEFAULT:
   default:
      {
         ptr = allocator->alloc( size + headerSize );

         // insert the header
         *(int*)ptr = (int)allocator;
         ptr = (char*)ptr + headerSize;

         break;
      }
   }

   return ptr;
}

///////////////////////////////////////////////////////////////////////////////

void MemoryRouter::dealloc( void* ptr, AllocationMode allocMode )
{
   const size_t headerSize = sizeof( void* );

   switch( allocMode )
   {
   case AM_ALIGNED_16:
      {
         void* postHeaderPtr = MemoryUtils::resolveAlignedPointer( ptr );

         // decode the address of the allocator
         void* origPtr = (char*)postHeaderPtr - headerSize;
         int allocAddrRaw = *(int*)origPtr;
         MemoryAllocator* allocator = (MemoryAllocator*)allocAddrRaw;

         if ( origPtr )
         {
            allocator->dealloc( origPtr );
         }
         break;
      }


   case AM_DEFAULT:
   default:
      {
         // decode the address of the allocator
         void* origPtr = (char*)ptr - headerSize;
         int allocAddrRaw = *(int*)origPtr;
         MemoryAllocator* allocator = (MemoryAllocator*)allocAddrRaw;

         allocator->dealloc( origPtr );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void* MemoryRouter::convertAllocatedToObjectAddress( void* allocatedAddr, AllocationMode allocMode )
{
   const size_t headerSize = sizeof( void* );
   void* retAddress = NULL;

   switch( allocMode )
   {
   case AM_ALIGNED_16:
      {
         const int ALIGNMENT = 16;

         retAddress = (char*)allocatedAddr + headerSize;

         // then align the address
         retAddress = MemoryUtils::alignPointer( retAddress, ALIGNMENT );
         break;
      }


   case AM_DEFAULT:
   default:
      {
         retAddress = (char*)allocatedAddr + headerSize;

         break;
      }
   }

   return retAddress;
}

///////////////////////////////////////////////////////////////////////////////
