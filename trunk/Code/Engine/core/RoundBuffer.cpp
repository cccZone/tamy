#include "core.h"
#include "core/RoundBuffer.h"
#include "core\Assert.h"
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////

int RoundBuffer::BUFFER_INFO_CHUNK_SIZE = sizeof( size_t );

///////////////////////////////////////////////////////////////////////////////

RoundBuffer::RoundBuffer( size_t size )
{
   m_bufSize = size;
   m_memory = new char[ m_bufSize ];
   m_allocStartOffset = 0;
   m_allocEndOffset = 0;
   m_allocationsCount = 0;
}

///////////////////////////////////////////////////////////////////////////////

RoundBuffer::~RoundBuffer()
{
   delete [] m_memory;
   m_memory = NULL;
}

///////////////////////////////////////////////////////////////////////////////

size_t RoundBuffer::getMemoryUsed() const 
{ 
   return m_allocEndOffset - m_allocStartOffset; 
}

///////////////////////////////////////////////////////////////////////////////

void* RoundBuffer::alloc( size_t size )
{
   size_t sizeAtEnd = m_bufSize - m_allocEndOffset - BUFFER_INFO_CHUNK_SIZE;

   if ( size < sizeAtEnd )
   {
      // get the pointer to the allocated memory chunk
      char* ptr = m_memory + m_allocEndOffset;

      // memorize pointer to the buffer and the chunk size
      size += BUFFER_INFO_CHUNK_SIZE;
      *(size_t*)ptr = size;
      ptr += sizeof( size );

      // move the pointer to the free memory start
      m_allocEndOffset += size;

      ++m_allocationsCount;

      // return the pointer
      return ptr;
   }
   else
   {
      // there's not enough memory
      ASSERT_MSG( false, "Not enough memory in the round buffer" );
      return NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RoundBuffer::dealloc( void* ptr )
{
   if ( ptr != m_memory + m_allocStartOffset + BUFFER_INFO_CHUNK_SIZE )
   {
      ASSERT_MSG( false, "Trying to deallocate invalid address from the RoundBuffer." );
      return;
   }

   // read the size of the allocated chunk
   size_t deallocatedChunkSize = *(int*)( (char*)ptr - sizeof( size_t ) );

   // move the start pointer, keeping in mind to add the size chunk info size to it as well ( it was also a part of the allocation )
   m_allocStartOffset += deallocatedChunkSize;
   if ( m_allocStartOffset == m_allocEndOffset )
   {
      // ok - we freed everything we allocated - let's start over
      m_allocStartOffset = m_allocEndOffset = 0;
   }

   --m_allocationsCount;
}

///////////////////////////////////////////////////////////////////////////////
