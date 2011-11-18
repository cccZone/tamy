/// @file   core/MemoryPool.h
/// @brief  simple memory pool
#pragma once


///////////////////////////////////////////////////////////////////////////////

/**
 * A simple memory pool.
 */
class MemoryPool
{
private:
   static int     POOL_INFO_CHUNK_SIZE;

   size_t         m_bufSize;
   char*          m_memory;
   size_t         m_allocStartOffset;

   unsigned int   m_allocationsCount;

public:
   /**
    * Constructor.
    *
    * @param size          buffer size
    */
   MemoryPool( size_t size );
   ~MemoryPool();

   /**
    * Returns the amount of memory allocated.
    */
   size_t getMemoryUsed() const;

   /**
    * Returns the number of allocations made.
    */
   inline unsigned int getAllocationsCount() const { return m_allocationsCount; }

   /**
    * Resets the pool allocation pointer, making sure the objects
    * that will be allocated here from now on will be allocated
    * from the pool's memory block start.
    */
   void reset();

   /**
    * Extracts the pointer to the pool the object is stored in.
    *
    * @param ptr
    */
   static MemoryPool* getPoolPtr( void* ptr );

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

/**
 * An object we can allocate in a memory pool.
 */
class MemoryPoolObject
{
public:
   virtual ~MemoryPoolObject();

   /**
    * Placement allocator for the pool.
    */
   void* operator new( size_t size, MemoryPool& buffer );

   /**
    * Placement deallocator for the pool.
    */
   void operator delete( void* ptr, MemoryPool& buffer );

private:
   /**
    * Regular allocator for the pool - can't use it.
    */
   void* operator new( size_t size );

   /**
    * Regular deallocator - can't use it.
    */
   void operator delete( void* ptr );
};

///////////////////////////////////////////////////////////////////////////////

#define MEMPOOL_DELETE( Obj ) if ( Obj ) { static_cast< MemoryPoolObject* >(Obj)->~MemoryPoolObject(); Obj = NULL; }

///////////////////////////////////////////////////////////////////////////////