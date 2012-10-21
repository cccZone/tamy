#ifndef _ROUND_BUFFER_H
#error "This file can only be included from RoundBuffer.h"
#else

#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

unsigned int RoundBuffer::getAllocationsCount() const 
{ 
   return m_allocationsCount; 
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
T* RoundBuffer::front()
{
   if ( m_allocStartOffset == m_allocEndOffset )
   {
      return NULL;
   }
   else
   {
      char* startAddr = m_memory + m_allocStartOffset + BUFFER_INFO_CHUNK_SIZE;
      startAddr = (char*)MemoryRouter::convertAllocatedToObjectAddress( startAddr, GET_ALLOC_MODE( T ) );
      return reinterpret_cast< T* >( startAddr );
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _ROUND_BUFFER_H
