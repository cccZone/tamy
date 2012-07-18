#ifndef _RUNTIME_DATA_H
#error "This file can only be included from RuntimeData.h"
#else

#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

template< typename T >
TRuntimeVar< T >::TRuntimeVar()
{
   m_id = s_nextId++;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
unsigned int TRuntimeVar< T >::getId() const 
{ 
   return m_id; 
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
unsigned int TRuntimeVar< T >::getTypeSize() const 
{ 
   return sizeof( T ); 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
void RuntimeDataBuffer::registerVar( const typename TRuntimeVar< T >& var, const T& defaultVal )
{
   unsigned int size = var.getTypeSize();
   unsigned long newSize = m_size + size;

   if ( newSize > INITIAL_SIZE )
   {
      // we've exceeded the initial size of the buffer - which means we must grow it
      char* newBuf = new char[ newSize ];
      memcpy( newBuf, m_buffer, m_size );
      delete [] m_buffer;
      m_buffer = newBuf;
   }

   // initialize the new variable
   unsigned int varId = var.getId();
   VarsLayout::iterator it = m_varsLayout.find( varId );
   if ( it != m_varsLayout.end() )
   {
      ASSERT_MSG( false, "This runtime variable has already been registered." );
   }
   m_varsLayout.insert( std::make_pair( varId, m_size ) );

   // initialize the memory 
   memcpy( m_buffer + m_size, &defaultVal, size );

   // store the new size of the data held in the buffer
   m_size = newSize;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
T& RuntimeDataBuffer::operator[]( const typename TRuntimeVar< T >& var )
{
   unsigned int varId = var.getId();
   VarsLayout::iterator it = m_varsLayout.find( varId );
   if ( it == m_varsLayout.end() )
   {
      ASSERT_MSG( false,  "This runtime variable wasn't registered." );
   }

   char* data = m_buffer + it->second;
   return reinterpret_cast< T& >( *data );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const T& RuntimeDataBuffer::operator[]( const typename TRuntimeVar< T >& var ) const
{
   unsigned int varId = var.getId();
   VarsLayout::iterator it = m_varsLayout.find( varId );
   if ( it == m_varsLayout.end() )
   {
      ASSERT_MSG( false, "This runtime variable wasn't registered." );
   }

   char* data = m_buffer + it->second;
   return reinterpret_cast< T& >( *data );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _RUNTIME_DATA_H
