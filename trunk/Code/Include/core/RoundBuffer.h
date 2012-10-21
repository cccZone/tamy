/// @file   core/RoundBuffer.h
/// @brief  round buffer data structure
#ifndef _ROUND_BUFFER_H
#define _ROUND_BUFFER_H

#include "core\MemoryRouter.h"
#include "core\MemoryAllocator.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a structure similar to a queue, but it operates
 * on a constant area of memory, and thus is much much faster.
 */
class RoundBuffer : public MemoryAllocator
{
   DECLARE_ALLOCATOR( RoundBuffer, AM_DEFAULT );

private:
   static int     BUFFER_INFO_CHUNK_SIZE;
   size_t         m_bufSize;
   char*          m_memory;
   size_t         m_allocStartOffset;
   size_t         m_allocEndOffset;

   unsigned int   m_allocationsCount;

public:
   /**
    * Constructor.
    *
    * @param size          buffer size
    */
   RoundBuffer( size_t size );
   ~RoundBuffer();

   /**
    * Returns the amount of memory allocated.
    */
   size_t getMemoryUsed() const;

   /**
    * Returns the number of allocations made.
    */
   inline unsigned int getAllocationsCount() const;

   /**
    * Returns the front element.
    */
   template< typename T >
   T* front();

   // -------------------------------------------------------------------------
   // MemoryAllocator implementation
   // -------------------------------------------------------------------------
   void* alloc( size_t size );
   void dealloc( void* ptr );
};

///////////////////////////////////////////////////////////////////////////////

#include "core\RoundBuffer.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _ROUND_BUFFER_H
