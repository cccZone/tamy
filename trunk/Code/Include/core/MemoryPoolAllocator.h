/// @file   core/MemoryPoolAllocator.h
/// @brief  This allocator allocates data in a designated memory pool
#pragma once

#include "core\MemoryPool.h"
#include "core\MemoryAllocator.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This allocator allocates data in a designated memory pool.
 */
class MemoryPoolAllocator : public MemoryAllocator
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

   // -------------------------------------------------------------------------
   // MemoryAllocator implementation
   // -------------------------------------------------------------------------
   void* alloc( size_t size );
   void dealloc( void* ptr );
   ulong getMemoryUsed() const { return m_pool->getMemoryUsed(); }
};

///////////////////////////////////////////////////////////////////////////////
