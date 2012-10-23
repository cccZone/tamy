#ifndef _RUNTIME_DATA_H
#error "This file can only be included from RuntimeData.h"
#else

#include "core\Assert.h"
#include "core\MemoryUtils.h"


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
   const int ALIGNMENT = 16;
   ulong newEndAddress = MemoryUtils::calcAlignedSize( m_endAddress + var.getTypeSize(), ALIGNMENT );

   ASSERT_MSG( newEndAddress < BUFFER_SIZE, "Allocate a larger buffer" );
   if ( newEndAddress >= BUFFER_SIZE )
   {
      return;
   }

   // initialize the new variable
   unsigned int varId = var.getId();
   VarsLayout::iterator it = m_varsLayout.find( varId );
   if ( it != m_varsLayout.end() )
   {
      ASSERT_MSG( false, "This runtime variable has already been registered." );
   }

   void* addr = (void*)( m_buffer + m_endAddress );
   void* alignedAddress = MemoryUtils::alignAddress( addr, ALIGNMENT );
   m_varsLayout.insert( std::make_pair( varId, (ulong)alignedAddress ) );

   // initialize the memory 
   new ( alignedAddress ) T( defaultVal );

   // store the new size of the data held in the buffer
   m_endAddress = newEndAddress;
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

   char* data = (char*)it->second;
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

   char* data = (char*)it->second;
   return reinterpret_cast< T& >( *data );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _RUNTIME_DATA_H
