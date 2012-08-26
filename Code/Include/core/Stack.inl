#ifndef _STACK_H
#error "This file can only be included from Stack.h"
#else

#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
Stack< T, TAllocator >::Stack( TAllocator* allocator ) 
   : m_top( NULL ) 
   , m_defaultAllocator( NULL )
   , m_allocator( allocator )
{

   if ( !m_allocator )
   {
      m_defaultAllocator = new TAllocator();
      m_allocator = m_defaultAllocator;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
Stack< T, TAllocator >::~Stack() 
{ 
   StackSeg* toRemove;
   for( StackSeg* elem = m_top; elem != NULL;  )
   {
      toRemove = elem;
      elem = elem->m_prev;
      m_allocator->destroy( toRemove );
   }

   m_top = NULL;

   delete m_defaultAllocator;
   m_defaultAllocator = NULL;

   m_allocator = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void Stack< T, TAllocator >::push( T elem )
{
   StackSeg* newSeg = new ( m_allocator ) StackSeg( m_top, elem );
   m_top = newSeg;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
T Stack< T, TAllocator >::pop()
{
   StackSeg* removedSeg = m_top;
   m_top = removedSeg->m_prev;
   removedSeg->m_prev = NULL;

   T elem = removedSeg->m_elem;
   m_allocator->destroy( removedSeg );

   return elem;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
const T& Stack< T, TAllocator >::top() const 
{ 
   return m_top->m_elem; 
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
T& Stack< T, TAllocator >::top() 
{ 
   return m_top->m_elem; 
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
bool Stack< T, TAllocator >::empty() const 
{
   return m_top == NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
Stack< T, TAllocator >::StackSeg::StackSeg( StackSeg* prev, T elem ) 
   : m_prev( prev ) 
   , m_elem( elem )
{}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
Stack< T, TAllocator >::StackSeg::~StackSeg() 
{ 
   m_prev = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void* Stack< T, TAllocator >::StackSeg::operator new( size_t size, TAllocator* allocator )
{
   return allocator->alloc( size );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void Stack< T, TAllocator >::StackSeg::operator delete( void* ptr, TAllocator* allocator )
{
   allocator->dealloc( ptr );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void* Stack< T, TAllocator >::StackSeg::operator new( size_t size )
{
   ASSERT_MSG( false, "Stack element can only be allocated using a dedicated allocator." );
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T, typename TAllocator >
void Stack< T, TAllocator >::StackSeg::operator delete( void* ptr )
{
   ASSERT_MSG( false, "Stack element can only be allocated using a dedicated allocator." );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _STACK_H
