#include "core.h"
#include "core/MemoryPoolAllocator.h"


///////////////////////////////////////////////////////////////////////////////

MemoryPoolAllocator::MemoryPoolAllocator( MemoryPool* pool )
   : m_defaultPool( 0 )
   , m_pool( pool )
{
   if ( !m_pool )
   {
      m_defaultPool = new MemoryPool( 1024 );
      m_pool = m_defaultPool;
   }
}

///////////////////////////////////////////////////////////////////////////////

MemoryPoolAllocator::~MemoryPoolAllocator()
{
   delete m_defaultPool;
   m_defaultPool = 0;
   m_pool = 0;
}

///////////////////////////////////////////////////////////////////////////////

void* MemoryPoolAllocator::alloc( size_t size )
{
   void* ptr = m_pool->alloc( size );
   return ptr;
}

///////////////////////////////////////////////////////////////////////////////

void MemoryPoolAllocator::dealloc( void* ptr )
{
   m_pool->dealloc( ptr );
}

///////////////////////////////////////////////////////////////////////////////
