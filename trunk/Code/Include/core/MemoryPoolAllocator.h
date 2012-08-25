/// @file   core/MemoryPoolAllocator.h
/// @brief  This allocator allocates data in a designated memory pool
#pragma once

#include "core/MemoryPool.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This allocator allocates data in a designated memory pool.
 */
class MemoryPoolAllocator
{
private:
   MemoryPool*   m_defaultPool;
   MemoryPool*   m_pool;

public:
   /**
    * Constructor.
    *
    * @param pool
    */
   MemoryPoolAllocator( MemoryPool* pool = 0 );
   ~MemoryPoolAllocator();

   /**
    * Destroys an object allocated by the allocator
    */
   template< typename T >
   void destroy( T* obj )
   {
      if ( obj )
      {
         obj->~T();
         m_pool->dealloc( obj );
      }
   }

   /**
    * Allocates a chunk of memory of the specified size.
    *
    * @param size
    */
   void* alloc( size_t size );

   /**
    * Deallocates the memory from the specified address.
    */
   void dealloc( void* ptr );
};

///////////////////////////////////////////////////////////////////////////////
