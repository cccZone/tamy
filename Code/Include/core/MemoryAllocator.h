/// @file   core/MemoryAllocator.h
/// @brief  memory allocator interface
#pragma once


///////////////////////////////////////////////////////////////////////////////

class MemoryAllocator
{
public:
   virtual ~MemoryAllocator() {}

   /**
    * Allocates a chunk of memory of the specified size.
    *
    * @param size
    */
   virtual void* alloc( size_t size ) = 0;

   /**
    * Deallocates the memory from the specified address.
    */
   virtual void dealloc( void* ptr ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
