/// @file   core/RoundBuffer.h
/// @brief  round buffer data structure
#ifndef _ROUND_BUFFER_H
#define _ROUND_BUFFER_H


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a structure similar to a queue, but it operates
 * on a constant area of memory, and thus is much much faster.
 */
class RoundBuffer
{
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

   /**
    * Extracts the pointer to the buffer the object is stored in.
    *
    * @param ptr
    */
   static RoundBuffer* getBufferPtr( void* ptr );

   // ----------------------------------------------------------------------
   // MemoryPool implementation
   // ----------------------------------------------------------------------
   void* alloc( size_t size );
   void dealloc( void* ptr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * An object that can be allocated in a round buffer.
 */
class RoundBufferObject
{
public:
   virtual ~RoundBufferObject();

   /**
    * Placement allocator for the buffer.
    */
   void* operator new( size_t size, RoundBuffer& buffer );

   /**
    * Placement deallocator for the buffer.
    */
   void operator delete( void* ptr, RoundBuffer& buffer );

private:
   /**
    * Regular allocator for the buffer - can't use it.
    */
   void* operator new( size_t size );

   /**
    * Regular deallocator - can't use it.
    */
   void operator delete( void* ptr );
};

///////////////////////////////////////////////////////////////////////////////

#include "core\RoundBuffer.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _ROUND_BUFFER_H
