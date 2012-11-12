#include "core.h"
#include "core\MemoryRouter.h"
#include "core\CallstackTree.h"
#include "core\CallstackTracer.h"
#include "core\dostream.h"
#include <strstream>


///////////////////////////////////////////////////////////////////////////////

MemoryRouter* MemoryRouter::s_theInstance = NULL;

///////////////////////////////////////////////////////////////////////////////

MemoryRouter::MemoryRouter()
{
#ifdef _TRACK_MEMORY_ALLOCATIONS
   m_tracer = new CallstackTracer();
   m_callstacksTree = new CallstackTree();
#endif
}

///////////////////////////////////////////////////////////////////////////////

MemoryRouter::~MemoryRouter()
{
#ifdef _TRACK_MEMORY_ALLOCATIONS
   delete m_tracer;
   m_tracer = NULL;

   delete m_callstacksTree;
   m_callstacksTree = NULL;
#endif
}

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

void MemoryRouter::deinitialize()
{
#ifdef _TRACK_MEMORY_ALLOCATIONS

   // dump unallocated callstacks
   {
      dostream dbgOutput;
      uint callstacksCount = s_theInstance->m_callstacksTree->getCallstacksCount();
      if ( callstacksCount > 0 )
      {
         dbgOutput << 
            "\n\n===================================================================\n" <<
            " MEMORY LEAKS REPORT - " << callstacksCount << " leaks encountered, " << s_theInstance->m_defaultAllocator.getMemoryUsed() << " bytes leaked \n" <<
            "===================================================================\n\n";

         ulong callstackTrace[128];
         for ( uint i = 0; i < callstacksCount; ++i )
         {
            uint callstackId = s_theInstance->m_callstacksTree->getCallstackId( i );
            uint callstackSize = s_theInstance->m_callstacksTree->getCallstack( callstackId, callstackTrace, 128 );
            s_theInstance->m_tracer->printCallstack( dbgOutput, callstackTrace, callstackSize );
         }

         dbgOutput << 
            "\n\n===================================================================\n" <<
            " MEMORY LEAKS REPORT END - " << callstacksCount << " leaks encountered\n" <<
            "===================================================================\n\n";
      }
      else
      {
         dbgOutput << 
            "\n\n===================================================================\n" <<
            " No memory leaks detected\n" <<
            "===================================================================\n\n";
      }

      dbgOutput.flush();
   }

#endif

   delete s_theInstance;
   s_theInstance = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void* MemoryRouter::alloc( size_t size, AllocationMode allocMode, MemoryAllocator* allocator )
{
   const size_t headerSize = sizeof( void* );

   const int ALIGNMENT = 16;
   size_t alignedSize = MemoryUtils::calcAlignedSize( size, ALIGNMENT );
   void* pa = allocator->alloc( alignedSize + headerSize );

   if( !pa )
   {
      return NULL;
   }

#ifdef _TRACK_MEMORY_ALLOCATIONS
   {
      // memorize the callstack
      ulong callstack[64];
      uint callstackSize = m_tracer->getStackTrace( callstack, 64 );
      m_callstacksTree->insert( (uint)pa, callstack, callstackSize );
   }
#endif

   // first - insert the header
   *(int*)pa = (int)allocator;
   pa = (char*)pa + headerSize;

   // then align the address
   void* ptr = MemoryUtils::alignAddressAndStoreOriginal( pa, ALIGNMENT );
         

   return ptr;
}

///////////////////////////////////////////////////////////////////////////////

void MemoryRouter::dealloc( void* ptr, AllocationMode allocMode )
{
   const size_t headerSize = sizeof( void* );
   void* postHeaderPtr = MemoryUtils::resolveAlignedAddress( ptr );

   // decode the address of the allocator
   void* origPtr = (char*)postHeaderPtr - headerSize;
   int allocAddrRaw = *(int*)origPtr;
   MemoryAllocator* allocator = (MemoryAllocator*)allocAddrRaw;

   if ( origPtr )
   {
#ifdef _TRACK_MEMORY_ALLOCATIONS
      // remove the callstack
      m_callstacksTree->remove( (uint)origPtr );
#endif

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
   retAddress = MemoryUtils::alignAddress( retAddress, ALIGNMENT );
   return retAddress;
}

///////////////////////////////////////////////////////////////////////////////
